//
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#include <omnetpp.h>
#include <string.h>

#include "ICMP.h"

Define_Module(ICMP);


void ICMP::endService(cMessage *msg)
{
    cGate *arrivalGate = msg->arrivalGate();

    // process arriving ICMP message
    if (!strcmp(arrivalGate->name(), "localIn"))
    {
        processICMPMessage((IPInterfacePacket *)msg);
        return;
    }

    // request from application
    if (!strcmp(arrivalGate->name(), "pingIn"))
    {
        sendEchoRequest(msg);
        return;
    }
}


void ICMP::sendErrorMessage(IPDatagram *origDatagram, ICMPType type, ICMPCode code)
{
    Enter_Method("sendErrorMessage(datagram, type=%d, code=%d)", type, code);

    // get ownership
    take(origDatagram);

    // don't send ICMP error messages for multicast messages
    if (origDatagram->destAddress().isMulticast())
    {
        ev << "won't send ICMP error messages for multicast message " << origDatagram << endl;
        delete origDatagram;
        return;
    }

    // do not reply with error message to error message
    if (origDatagram->protocol() == IP_PROT_ICMP)
    {
        ICMPMessage *recICMPMsg = check_and_cast<ICMPMessage *>(origDatagram->encapsulatedMsg());
        if (recICMPMsg->getIsError())
        {
            ev << "do not reply with error message to error message" << endl;
            delete origDatagram;
            return;
        }
    }

    ICMPMessage *errorMessage = new ICMPMessage();
    IPDatagram *e = (IPDatagram *)origDatagram->dup();

    errorMessage->setType(type);
    errorMessage->setCode(code);
    errorMessage->setIsError(true);
    errorMessage->encapsulate(e);
    // ICMP message length: see above
    errorMessage->setLength(8 * (4 + e->headerLength() + 8));

    // origDatagram should get deleted here, e not
    delete origDatagram;

    sendInterfacePacket(errorMessage, e->srcAddress());

    // debugging information
    ev << "sending error message: " << errorMessage->getType() << " / " << errorMessage->getCode() << endl;
}


//----------------------------------------------------------
// Private Functions
//----------------------------------------------------------

// error Messages are simply forwarded to errorOut
void ICMP::processICMPMessage(IPInterfacePacket *interfacePacket)
{
    ICMPMessage *icmpmsg = check_and_cast<ICMPMessage *>(interfacePacket->decapsulate());

    // use source of ICMP message as destination for reply
    IPAddress src = interfacePacket->srcAddr();
    delete interfacePacket;

    switch (icmpmsg->getType())
    {
        case ICMP_ECHO_REPLY:
            recEchoReply(icmpmsg);
            break;
        case ICMP_DESTINATION_UNREACHABLE:
            errorOut(icmpmsg);
            break;
        case ICMP_REDIRECT:
            errorOut(icmpmsg);
            break;
        case ICMP_ECHO_REQUEST:
            recEchoRequest(icmpmsg, src);
            break;
        case ICMP_TIME_EXCEEDED:
            errorOut(icmpmsg);
            break;
        case ICMP_PARAMETER_PROBLEM:
            errorOut(icmpmsg);
            break;
        case ICMP_TIMESTAMP_REQUEST:
            recEchoRequest(icmpmsg, src);
            break;
        case ICMP_TIMESTAMP_REPLY:
            recEchoReply(icmpmsg);
            break;
        default:
            opp_error("Unknown ICMP type %d", icmpmsg->getType());
    }
}

void ICMP::errorOut(ICMPMessage *icmpmsg)
{
    send(icmpmsg, "errorOut");
}


//----------------------------------------------------------
// Echo/Timestamp request and reply ICMP messages
//----------------------------------------------------------

void ICMP::recEchoRequest(ICMPMessage *request, const IPAddress& dest)
{
    ICMPMessage *reply = new ICMPMessage(*request);
    bool timestampValid = request->getQuery().getIsTimestampValid();

    reply->setType(timestampValid ? ICMP_TIMESTAMP_REPLY : ICMP_ECHO_REPLY);
    reply->setCode(0);
    // FIXME query must be copied too. Was: reply->setQuery( new ICMPQuery(*(request->query)) );
    reply->setLength(8*20);

    delete request;

    if (timestampValid)
    {
        reply->getQuery().setReceiveTimestamp(simTime());
        reply->getQuery().setTransmitTimestamp(simTime());
    }

    sendInterfacePacket(reply, dest);
}

void ICMP::recEchoReply (ICMPMessage *reply)
{
    cMessage *msg = new cMessage;
    ICMPQuery *echoInfo = new ICMPQuery(reply->getQuery());

    delete reply;

    //FIXME add back next line ASAP
    //msg->parList().add( echoInfo );
    send(msg, "pingOut");
}

void ICMP::sendEchoRequest(cMessage *msg)
{
    ICMPMessage *icmpmsg = new ICMPMessage();
    ICMPQuery *echoInfo = (ICMPQuery *)msg->parList().get("echoInfo");

    icmpmsg->setType(echoInfo->getIsTimestampValid() ? ICMP_TIMESTAMP_REQUEST : ICMP_ECHO_REQUEST);
    icmpmsg->setCode(0);
    // FIXME query must be copied too. Was: icmpmsg->query = new ICMPQuery(*echoInfo);
    icmpmsg->setLength(8*20);

    IPAddress dest = msg->par("destination_address").stringValue();
    delete msg;

    sendInterfacePacket(icmpmsg, dest);
}

void ICMP::sendInterfacePacket(ICMPMessage *msg, const IPAddress& dest)
{
    IPInterfacePacket *interfacePacket = new IPInterfacePacket;

    interfacePacket->encapsulate(msg);
    interfacePacket->setDestAddr(dest);
    interfacePacket->setProtocol(IP_PROT_ICMP);

    send(interfacePacket, "sendOut");
}


