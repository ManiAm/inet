//
// Copyright (C) 2006 Andras Babos and Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "inet/routing/ospfv2/OspfCrc.h"
#include "inet/routing/ospfv2/messagehandler/LinkStateUpdateHandler.h"
#include "inet/routing/ospfv2/neighbor/OspfNeighbor.h"
#include "inet/routing/ospfv2/router/OspfArea.h"
#include "inet/routing/ospfv2/router/OspfCommon.h"
#include "inet/routing/ospfv2/router/OspfRouter.h"

namespace inet {

namespace ospf {

class LsaProcessingMarker
{
  private:
    unsigned int index;

  public:
    LsaProcessingMarker(unsigned int counter) : index(counter) { EV_INFO << "    --> Processing LSA(" << index << ")\n"; }
    ~LsaProcessingMarker() { EV_INFO << "    <-- LSA(" << index << ") processed.\n"; }
};

LinkStateUpdateHandler::LinkStateUpdateHandler(Router *containingRouter) :
    IMessageHandler(containingRouter)
{
}

/**
 * @see RFC2328 Section 13.
 */
void LinkStateUpdateHandler::processPacket(Packet *packet, OspfInterface *intf, Neighbor *neighbor)
{
    router->getMessageHandler()->printEvent("Link State update packet received", intf, neighbor);

    const auto& lsUpdatePacket = packet->peekAtFront<OspfLinkStateUpdatePacket>();
    bool shouldRebuildRoutingTable = false;

    if (neighbor->getState() >= Neighbor::EXCHANGE_STATE) {
        AreaId areaID = lsUpdatePacket->getAreaID();
        Area *area = router->getAreaByID(areaID);

        EV_INFO << "  Processing packet contents:\n";

        for (unsigned int i = 0; i < lsUpdatePacket->getOspfLSAsArraySize(); i++) {
            const OspfLsa *currentLSA = lsUpdatePacket->getOspfLSAs(i);

            if (!validateLSChecksum(currentLSA)) {
                continue;
            }

            LsaType lsaType = static_cast<LsaType>(currentLSA->getHeader().getLsType());
            if ((lsaType != ROUTERLSA_TYPE) &&
                (lsaType != NETWORKLSA_TYPE) &&
                (lsaType != SUMMARYLSA_NETWORKS_TYPE) &&
                (lsaType != SUMMARYLSA_ASBOUNDARYROUTERS_TYPE) &&
                (lsaType != AS_EXTERNAL_LSA_TYPE))
            {
                continue;
            }

            LsaProcessingMarker marker(i);
            EV_DETAIL << "    " << currentLSA->getHeader() << "\n";

            //FIXME area maybe nullptr
            if ((lsaType == AS_EXTERNAL_LSA_TYPE) && !(area != nullptr && area->getExternalRoutingCapability())) {
                continue;
            }
            LsaKeyType lsaKey;

            lsaKey.linkStateID = currentLSA->getHeader().getLinkStateID();
            lsaKey.advertisingRouter = currentLSA->getHeader().getAdvertisingRouter();

            OspfLsa *lsaInDatabase = router->findLSA(lsaType, lsaKey, areaID);
            unsigned short lsAge = currentLSA->getHeader().getLsAge();
            AcknowledgementFlags ackFlags;

            ackFlags.floodedBackOut = false;
            ackFlags.lsaIsNewer = false;
            ackFlags.lsaIsDuplicate = false;
            ackFlags.impliedAcknowledgement = false;
            ackFlags.lsaReachedMaxAge = (lsAge == MAX_AGE);
            ackFlags.noLSAInstanceInDatabase = (lsaInDatabase == nullptr);
            ackFlags.anyNeighborInExchangeOrLoadingState = router->hasAnyNeighborInStates(Neighbor::EXCHANGE_STATE | Neighbor::LOADING_STATE);

            if ((ackFlags.lsaReachedMaxAge) && (ackFlags.noLSAInstanceInDatabase) && (!ackFlags.anyNeighborInExchangeOrLoadingState)) {
                if (intf->getType() == OspfInterface::BROADCAST) {
                    if ((intf->getState() == OspfInterface::DESIGNATED_ROUTER_STATE) ||
                        (intf->getState() == OspfInterface::BACKUP_STATE) ||
                        (intf->getDesignatedRouter() == NULL_DESIGNATEDROUTERID))
                    {
                        intf->sendLsAcknowledgement(&(currentLSA->getHeader()), Ipv4Address::ALL_OSPF_ROUTERS_MCAST);
                    }
                    else {
                        intf->sendLsAcknowledgement(&(currentLSA->getHeader()), Ipv4Address::ALL_OSPF_DESIGNATED_ROUTERS_MCAST);
                    }
                }
                else {
                    if (intf->getType() == OspfInterface::POINTTOPOINT) {
                        intf->sendLsAcknowledgement(&(currentLSA->getHeader()), Ipv4Address::ALL_OSPF_ROUTERS_MCAST);
                    }
                    else {
                        intf->sendLsAcknowledgement(&(currentLSA->getHeader()), neighbor->getAddress());
                    }
                }
                continue;
            }

            if (!ackFlags.noLSAInstanceInDatabase) {
                // operator< and operator== on OSPFLSAHeaders determines which one is newer(less means older)
                ackFlags.lsaIsNewer = (lsaInDatabase->getHeader() < currentLSA->getHeader());
                ackFlags.lsaIsDuplicate = (operator==(lsaInDatabase->getHeader(), currentLSA->getHeader()));
            }
            if ((ackFlags.noLSAInstanceInDatabase) || (ackFlags.lsaIsNewer)) {
                LsaTrackingInfo *info = (!ackFlags.noLSAInstanceInDatabase) ? dynamic_cast<LsaTrackingInfo *>(lsaInDatabase) : nullptr;
                if ((!ackFlags.noLSAInstanceInDatabase) &&
                    (info != nullptr) &&
                    (info->getSource() == LsaTrackingInfo::FLOODED) &&
                    (info->getInstallTime() < MIN_LS_ARRIVAL))
                {
                    continue;
                }
                ackFlags.floodedBackOut = router->floodLSA(currentLSA, areaID, intf, neighbor);
                if (!ackFlags.noLSAInstanceInDatabase) {
                    LsaKeyType lsaKey;

                    lsaKey.linkStateID = lsaInDatabase->getHeader().getLinkStateID();
                    lsaKey.advertisingRouter = lsaInDatabase->getHeader().getAdvertisingRouter();

                    router->removeFromAllRetransmissionLists(lsaKey);
                }
                shouldRebuildRoutingTable |= router->installLSA(currentLSA, areaID);

                EV_INFO << "    (update installed)\n";

                acknowledgeLSA(currentLSA->getHeader(), intf, ackFlags, lsUpdatePacket->getRouterID());
                if ((currentLSA->getHeader().getAdvertisingRouter() == router->getRouterID()) ||
                    ((lsaType == NETWORKLSA_TYPE) &&
                     (router->isLocalAddress(currentLSA->getHeader().getLinkStateID()))))
                {
                    if (ackFlags.noLSAInstanceInDatabase) {
                        auto lsaCopy = currentLSA->dup();
                        lsaCopy->getHeaderForUpdate().setLsAge(MAX_AGE);
                        router->floodLSA(lsaCopy, areaID);
                    }
                    else {
                        if (ackFlags.lsaIsNewer) {
                            long sequenceNumber = currentLSA->getHeader().getLsSequenceNumber();
                            if (sequenceNumber == MAX_SEQUENCE_NUMBER) {
                                lsaInDatabase->getHeaderForUpdate().setLsAge(MAX_AGE);
                                router->floodLSA(lsaInDatabase, areaID);
                            }
                            else {
                                lsaInDatabase->getHeaderForUpdate().setLsSequenceNumber(sequenceNumber + 1);
                                router->floodLSA(lsaInDatabase, areaID);
                            }
                        }
                    }
                }
                continue;
            }
            if (neighbor->isLSAOnRequestList(lsaKey)) {
                neighbor->processEvent(Neighbor::BAD_LINK_STATE_REQUEST);
                break;
            }
            if (ackFlags.lsaIsDuplicate) {
                if (neighbor->isLinkStateRequestListEmpty(lsaKey)) {
                    neighbor->removeFromRetransmissionList(lsaKey);
                    ackFlags.impliedAcknowledgement = true;
                }
                acknowledgeLSA(currentLSA->getHeader(), intf, ackFlags, lsUpdatePacket->getRouterID());
                continue;
            }
            if ((lsaInDatabase->getHeader().getLsAge() == MAX_AGE) &&
                (lsaInDatabase->getHeader().getLsSequenceNumber() == MAX_SEQUENCE_NUMBER))
            {
                continue;
            }
            if (!neighbor->isOnTransmittedLSAList(lsaKey)) {
                Packet *updatePacket = intf->createUpdatePacket(lsaInDatabase);
                if (updatePacket != nullptr) {
                    int ttl = (intf->getType() == OspfInterface::VIRTUAL) ? VIRTUAL_LINK_TTL : 1;

                    if (intf->getType() == OspfInterface::BROADCAST) {
                        if ((intf->getState() == OspfInterface::DESIGNATED_ROUTER_STATE) ||
                            (intf->getState() == OspfInterface::BACKUP_STATE) ||
                            (intf->getDesignatedRouter() == NULL_DESIGNATEDROUTERID))
                        {
                            router->getMessageHandler()->sendPacket(updatePacket, Ipv4Address::ALL_OSPF_ROUTERS_MCAST, intf, ttl);
                        }
                        else {
                            router->getMessageHandler()->sendPacket(updatePacket, Ipv4Address::ALL_OSPF_DESIGNATED_ROUTERS_MCAST, intf, ttl);
                        }
                    }
                    else {
                        if (intf->getType() == OspfInterface::POINTTOPOINT) {
                            router->getMessageHandler()->sendPacket(updatePacket, Ipv4Address::ALL_OSPF_ROUTERS_MCAST, intf, ttl);
                        }
                        else {
                            router->getMessageHandler()->sendPacket(updatePacket, neighbor->getAddress(), intf, ttl);
                        }
                    }
                }
            }
        }
    }

    if (shouldRebuildRoutingTable)
        router->rebuildRoutingTable();
}

void LinkStateUpdateHandler::acknowledgeLSA(const OspfLsaHeader& lsaHeader,
        OspfInterface *intf,
        LinkStateUpdateHandler::AcknowledgementFlags acknowledgementFlags,
        RouterId lsaSource)
{
    bool sendDirectAcknowledgment = false;

    if (!acknowledgementFlags.floodedBackOut) {
        if (intf->getState() == OspfInterface::BACKUP_STATE) {
            if ((acknowledgementFlags.lsaIsNewer && (lsaSource == intf->getDesignatedRouter().routerID)) ||
                (acknowledgementFlags.lsaIsDuplicate && acknowledgementFlags.impliedAcknowledgement))
            {
                intf->addDelayedAcknowledgement(lsaHeader);
            }
            else {
                if ((acknowledgementFlags.lsaIsDuplicate && !acknowledgementFlags.impliedAcknowledgement) ||
                    (acknowledgementFlags.lsaReachedMaxAge &&
                     acknowledgementFlags.noLSAInstanceInDatabase &&
                     acknowledgementFlags.anyNeighborInExchangeOrLoadingState))
                {
                    sendDirectAcknowledgment = true;
                }
            }
        }
        else {
            if (acknowledgementFlags.lsaIsNewer) {
                intf->addDelayedAcknowledgement(lsaHeader);
            }
            else {
                if ((acknowledgementFlags.lsaIsDuplicate && !acknowledgementFlags.impliedAcknowledgement) ||
                    (acknowledgementFlags.lsaReachedMaxAge &&
                     acknowledgementFlags.noLSAInstanceInDatabase &&
                     acknowledgementFlags.anyNeighborInExchangeOrLoadingState))
                {
                    sendDirectAcknowledgment = true;
                }
            }
        }
    }

    if (sendDirectAcknowledgment) {
        const auto& ackPacket = makeShared<OspfLinkStateAcknowledgementPacket>();

        ackPacket->setType(LINKSTATE_ACKNOWLEDGEMENT_PACKET);
        ackPacket->setRouterID(Ipv4Address(router->getRouterID()));
        ackPacket->setAreaID(Ipv4Address(intf->getArea()->getAreaID()));
        ackPacket->setAuthenticationType(intf->getAuthenticationType());

        ackPacket->setLsaHeadersArraySize(1);
        ackPacket->setLsaHeaders(0, lsaHeader);

        ackPacket->setChunkLength(OSPF_HEADER_LENGTH + OSPF_LSA_HEADER_LENGTH);

        AuthenticationKeyType authKey = intf->getAuthenticationKey();
        for (int i = 0; i < 8; i++) {
            ackPacket->setAuthentication(i, authKey.bytes[i]);
        }

        setOspfCrc(ackPacket, intf->getCrcMode());

        Packet *pk = new Packet();
        pk->insertAtBack(ackPacket);

        int ttl = (intf->getType() == OspfInterface::VIRTUAL) ? VIRTUAL_LINK_TTL : 1;

        if (intf->getType() == OspfInterface::BROADCAST) {
            if ((intf->getState() == OspfInterface::DESIGNATED_ROUTER_STATE) ||
                (intf->getState() == OspfInterface::BACKUP_STATE) ||
                (intf->getDesignatedRouter() == NULL_DESIGNATEDROUTERID))
            {
                router->getMessageHandler()->sendPacket(pk, Ipv4Address::ALL_OSPF_ROUTERS_MCAST, intf, ttl);
            }
            else {
                router->getMessageHandler()->sendPacket(pk, Ipv4Address::ALL_OSPF_DESIGNATED_ROUTERS_MCAST, intf, ttl);
            }
        }
        else {
            if (intf->getType() == OspfInterface::POINTTOPOINT) {
                router->getMessageHandler()->sendPacket(pk, Ipv4Address::ALL_OSPF_ROUTERS_MCAST, intf, ttl);
            }
            else {
                Neighbor *neighbor = intf->getNeighborById(lsaSource);
                ASSERT(neighbor);
                router->getMessageHandler()->sendPacket(pk, neighbor->getAddress(), intf, ttl);
            }
        }
    }
}

} // namespace ospf

} // namespace inet

