//
// Copyright 2008-2012 Irene Ruengeler
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "inet/applications/sctpapp/SctpNatPeer.h"
#include "inet/transportlayer/contract/sctp/SctpSocket.h"
#include "inet/transportlayer/contract/sctp/SctpCommand_m.h"
//#include "inet/transportlayer/sctp/SctpMessage_m.h"
#include <stdlib.h>
#include <stdio.h>
#include "inet/transportlayer/sctp/SctpAssociation.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/applications/common/SocketTag_m.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/packet/Message.h"

namespace inet {

using namespace sctp;

#define MSGKIND_CONNECT    0
#define MSGKIND_SEND       1
#define MSGKIND_ABORT      2
#define MSGKIND_PRIMARY    3
#define MSGKIND_RESET      4
#define MSGKIND_STOP       5

Define_Module(SctpNatPeer);

SctpNatPeer::SctpNatPeer()
{
    timeMsg = nullptr;
    timeoutMsg = nullptr;
    numSessions = 0;
    packetsSent = 0;
    packetsRcvd = 0;
    bytesSent = 0;
    notifications = 0;
    serverAssocId = 0;
    delay = 0;
    echo = false;
    schedule = false;
    shutdownReceived = false;
    sendAllowed = true;
    numRequestsToSend = 0;
    ordered = true;
    queueSize = 0;
    outboundStreams = 1;
    inboundStreams = 1;
    bytesRcvd = 0;
    echoedBytesSent = 0;
    lastStream = 0;
    chunksAbandoned = 0;
    numPacketsToReceive = 1;
    rendezvous = false;
    peerPort = 0;
}

SctpNatPeer::~SctpNatPeer()
{
    cancelAndDelete(timeMsg);
    cancelAndDelete(timeoutMsg);
}

void SctpNatPeer::initialize(int stage)
{
    cSimpleModule::initialize(stage);
    EV_DEBUG << "initialize SCTP NAT Peer stage " << stage << endl;
    if (stage == INITSTAGE_LOCAL) {
        WATCH(numSessions);
        WATCH(packetsSent);
        WATCH(packetsRcvd);
        WATCH(bytesSent);
        WATCH(numRequestsToSend);
        timeoutMsg = new cMessage("SrvAppTimer");
        queueSize = par("queueSize");
   } else if (stage == INITSTAGE_APPLICATION_LAYER) {
        // parameters
        const char *addressesString = par("localAddress");
        AddressVector addresses = L3AddressResolver().resolve(cStringTokenizer(addressesString).asVector());
        int32 port = par("localPort");
        echo = par("echo");
        delay = par("echoDelay");
        outboundStreams = par("outboundStreams");
        inboundStreams = par("inboundStreams");
        ordered = (bool)par("ordered");
        clientSocket.setOutputGate(gate("socketOut"));
        clientSocket.setCallbackObject(this);
        if (addresses.size() == 0) {
            clientSocket.bind(port);
        }
        else {
            clientSocket.bindx(addresses, port);
        }

        rendezvous = (bool)par("rendezvous");
        if ((simtime_t)par("startTime") > 0) {
            cMessage *msg = new cMessage("ConnectTimer");
            msg->setKind(MSGKIND_CONNECT);
            scheduleAt((simtime_t)par("startTime"), msg);
        }
    }
}

void SctpNatPeer::sendOrSchedule(cMessage *msg)
{
    if (delay == 0) {
        send(msg, "socketOut");
    }
    else {
        scheduleAt(simTime() + delay, msg);
    }
}

void SctpNatPeer::generateAndSend()
{
    auto applicationPacket = new Packet("ApplicationPacket");
    auto applicationData = makeShared<BytesChunk>();
    int numBytes = par("requestLength");
    EV_INFO << "Send " << numBytes << " bytes of data, bytesSent = " << bytesSent << endl;
    std::vector<uint8_t> vec;
    vec.resize(numBytes);
    for (int i = 0; i < numBytes; i++)
        vec[i] = (bytesSent + i) & 0xFF;
    applicationData->setBytes(vec);
    applicationPacket->insertAtBack(applicationData);
    auto sctpSendReq = applicationPacket->addTagIfAbsent<SctpSendReq>();
    sctpSendReq->setLast(true);
    sctpSendReq->setPrMethod(par("prMethod"));
    sctpSendReq->setPrValue(par("prValue"));
    lastStream = (lastStream + 1) % outboundStreams;
    sctpSendReq->setSid(lastStream);
    sctpSendReq->setSocketId(serverAssocId);
    auto creationTimeTag = applicationPacket->addTagIfAbsent<CreationTimeTag>();
    creationTimeTag->setCreationTime(simTime());
    applicationPacket->setKind(ordered ? SCTP_C_SEND_ORDERED : SCTP_C_SEND_UNORDERED);
    auto& tags = getTags(applicationPacket);
    tags.addTagIfAbsent<SocketReq>()->setSocketId(serverAssocId);
    tags.addTagIfAbsent<DispatchProtocolReq>()->setProtocol(&Protocol::sctp);
    bytesSent += numBytes;
    packetsSent++;
    clientSocket.sendMsg(applicationPacket);
}

void SctpNatPeer::connectx(AddressVector connectAddressList, int32 connectPort)
{
    uint32 outStreams = par("outboundStreams");
    clientSocket.setOutboundStreams(outStreams);
    uint32 inStreams = par("inboundStreams");
    clientSocket.setInboundStreams(inStreams);

    EV << "issuing OPEN command\n";
    EV << "Assoc " << clientSocket.getConnectionId() << "::connect to  port " << connectPort << "\n";
    bool streamReset = par("streamReset");
    clientSocket.connectx(connectAddressList, connectPort, streamReset, (int32)par("prMethod"), (uint32)par("numRequestsPerSession"));
    numSessions++;

    if (!streamReset)
        streamReset = false;
    else if (streamReset == true) {
        cMessage *cmsg = new cMessage("StreamReset");
        cmsg->setKind(MSGKIND_RESET);
        EV << "StreamReset Timer scheduled at " << simTime() << "\n";
        scheduleAt(simTime() + (double)par("streamRequestTime"), cmsg);
    }
    uint32 streamNum = 0;
    cStringTokenizer tokenizer(par("streamPriorities"));
    while (tokenizer.hasMoreTokens()) {
        const char *token = tokenizer.nextToken();
        clientSocket.setStreamPriority(streamNum, (uint32)atoi(token));

        streamNum++;
    }
}

void SctpNatPeer::connect(L3Address connectAddress, int32 connectPort)
{
    clientSocket.setOutboundStreams(outboundStreams);
    clientSocket.setInboundStreams(inboundStreams);

    EV << "issuing OPEN command\n";
    EV << "Assoc " << clientSocket.getConnectionId() << "::connect to address " << connectAddress << ", port " << connectPort << "\n";
    bool streamReset = par("streamReset");
    clientSocket.connect(connectAddress, connectPort, streamReset, (int32)par("prMethod"), (uint32)par("numRequestsPerSession"));
    numSessions++;

    if (!streamReset)
        streamReset = false;
    else if (streamReset == true) {
        cMessage *cmsg = new cMessage("StreamReset");
        cmsg->setKind(MSGKIND_RESET);
        EV << "StreamReset Timer scheduled at " << simTime() << "\n";
        scheduleAt(simTime() + (double)par("streamRequestTime"), cmsg);
    }
    uint32 streamNum = 0;
    cStringTokenizer tokenizer(par("streamPriorities"));
    while (tokenizer.hasMoreTokens()) {
        const char *token = tokenizer.nextToken();
        clientSocket.setStreamPriority(streamNum, (uint32)atoi(token));

        streamNum++;
    }
}

void SctpNatPeer::handleMessage(cMessage *msg)
{
    int32 id;

    if (msg->isSelfMessage()) {
        handleTimer(msg);
    }
    else {
        EV << "SctpNatPeer::handleMessage kind=" << SctpAssociation::indicationName(msg->getKind()) << " (" << msg->getKind() << ")\n";
        switch (msg->getKind()) {
            case SCTP_I_ADDRESS_ADDED:
                if (rendezvous)
                    clientSocket.processMessage(msg);
               // else
                delete msg;
                break;

            case SCTP_I_PEER_CLOSED:
            case SCTP_I_ABORT: {
                if (rendezvous)
                    clientSocket.processMessage(msg);
                else {
                    Message *message = check_and_cast<Message *>(msg);
                    auto& msgtags = getTags(message);
                    SctpCommandReq *command = msgtags.findTag<SctpCommandReq>();
                    Request *cmsg = new Request("SCTP_C_ABORT");
                    auto& tags = getTags(cmsg);
                    SctpSendReq *cmd = tags.addTagIfAbsent<SctpSendReq>();
                    int assocId = command->getSocketId();
                    tags.addTagIfAbsent<SocketReq>()->setSocketId(assocId);
                    tags.addTagIfAbsent<DispatchProtocolReq>()->setProtocol(&Protocol::sctp);
                    cmd->setSocketId(assocId);
                    cmd->setSid(command->getSid());
                    cmd->setNumMsgs(command->getNumMsgs());
                    cmsg->setKind(SCTP_C_ABORT);
                    delete msg;
                    sendOrSchedule(cmsg);
                }
                break;
            }

            case SCTP_I_ESTABLISHED: {
                if (clientSocket.getState() == SctpSocket::CONNECTING) {
                    clientSocket.processMessage(msg);
                } else {
                    int32 count = 0;
                    Message *message = check_and_cast<Message *>(msg);
                    auto& tags = getTags(message);
                    SctpConnectReq *connectInfo = tags.findTag<SctpConnectReq>();
                    numSessions++;
                    serverAssocId = connectInfo->getSocketId();
                    id = serverAssocId;
                    outboundStreams = connectInfo->getOutboundStreams();
                    inboundStreams = connectInfo->getInboundStreams();
                    rcvdPacketsPerAssoc[serverAssocId] = (int64)(long)par("numPacketsToReceivePerClient");
                    sentPacketsPerAssoc[serverAssocId] = (int64)(long)par("numPacketsToSendPerClient");
                    char text[128];
                    sprintf(text, "App: Received Bytes of assoc %d", serverAssocId);
                    bytesPerAssoc[serverAssocId] = new cOutVector(text);
                    rcvdBytesPerAssoc[serverAssocId] = 0;
                    sprintf(text, "App: EndToEndDelay of assoc %d", serverAssocId);
                    endToEndDelay[serverAssocId] = new cOutVector(text);
                    sprintf(text, "Hist: EndToEndDelay of assoc %d", serverAssocId);
                    histEndToEndDelay[serverAssocId] = new cHistogram(text);

                    delete msg;

                    if ((int64)(long)par("numPacketsToSendPerClient") > 0) {
                        auto i = sentPacketsPerAssoc.find(serverAssocId);
                        numRequestsToSend = i->second;
                        if ((simtime_t)par("thinkTime") > 0) {
                            generateAndSend();
                            timeoutMsg->setKind(SCTP_C_SEND);
                            scheduleAt(simTime() + (simtime_t)par("thinkTime"), timeoutMsg);
                            numRequestsToSend--;
                            i->second = numRequestsToSend;
                        }
                        else {
                            if (queueSize == 0) {
                                while (numRequestsToSend > 0) {
                                    generateAndSend();
                                    numRequestsToSend--;
                                    i->second = numRequestsToSend;
                                }
                            }
                            else if (queueSize > 0) {
                                while (numRequestsToSend > 0 && count++ < queueSize * 2) {
                                    generateAndSend();
                                    numRequestsToSend--;
                                    i->second = numRequestsToSend;
                                }

                                Request *cmsg = new Request("SCTP_C_QUEUE_MSGS_LIMIT");
                                auto& tags = getTags(cmsg);
                                SctpInfoReq *qinfo = tags.addTagIfAbsent<SctpInfoReq>();
                                qinfo->setText(queueSize);
                                cmsg->setKind(SCTP_C_QUEUE_MSGS_LIMIT);
                                qinfo->setSocketId(id);
                                sendOrSchedule(cmsg);
                            }

                            EV << "!!!!!!!!!!!!!!!All data sent from Peer !!!!!!!!!!\n";

                            auto j = rcvdPacketsPerAssoc.find(serverAssocId);
                            if (j->second == 0 && (simtime_t)par("waitToClose") > 0) {
                                char as[5];
                                sprintf(as, "%d", serverAssocId);
                                cMessage *abortMsg = new cMessage(as);
                                abortMsg->setKind(SCTP_I_ABORT);
                                scheduleAt(simTime() + (simtime_t)par("waitToClose"), abortMsg);
                            }
                            else {
                                EV << "no more packets to send, call shutdown for assoc " << serverAssocId << "\n";
                                Request *cmsg = new Request("ShutdownRequest");
                                auto& tags = getTags(cmsg);
                                SctpCommandReq *cmd = tags.addTagIfAbsent<SctpCommandReq>();
                                cmsg->setKind(SCTP_C_SHUTDOWN);
                                cmd->setSocketId(serverAssocId);
                                sendOrSchedule(cmsg);
                            }
                        }
                    }
                }
                break;
            }

            case SCTP_I_DATA_NOTIFICATION: {
                EV_DETAIL << "NatPeer: SCTP_I_DATA_NOTIFICATION arrived\n";
                notifications++;
                Message *message = check_and_cast<Message *>(msg);
                auto& intags = getTags(message);
                SctpCommandReq *ind = intags.findTag<SctpCommandReq>();
                id = ind->getSocketId();
                Request *cmsg = new Request("ReceiveRequest");
                auto& outtags = getTags(cmsg);
                auto cmd = outtags.addTagIfAbsent<SctpSendReq>();
                outtags.addTagIfAbsent<SocketReq>()->setSocketId(id);
                outtags.addTagIfAbsent<DispatchProtocolReq>()->setProtocol(&Protocol::sctp);
                cmd->setSocketId(id);
                cmd->setSid(ind->getSid());
                cmd->setNumMsgs(ind->getNumMsgs());
                cmsg->setKind(SCTP_C_RECEIVE);
                delete msg;
                if (!cmsg->isScheduled() && schedule == false) {
                    scheduleAt(simTime() + (simtime_t)par("delayFirstRead"), cmsg);
                }
                else if (schedule == true)
                    sendOrSchedule(cmsg);
                break;
            }

            case SCTP_I_DATA: {
                Packet *message = check_and_cast<Packet *>(msg);
                auto& tags = getTags(message);
                SctpRcvReq *ind = tags.findTag<SctpRcvReq>();
                id = ind->getSocketId();
                if (rendezvous) {
                    const auto& smsg = staticPtrCast<const BytesChunk>(message->peekData());
                    int bufferlen = B(smsg->getChunkLength()).get();
                    uint8_t buffer[bufferlen];
                    std::vector<uint8_t> vec = smsg->getBytes();
                    for (unsigned int i = 0; i < bufferlen; i++) {
                        buffer[i] = vec[i];
                    }
                    struct nat_message *nat = (struct nat_message *) buffer;
                    peerAddressList.clear();
                    if (nat->multi) {
                        peerAddressList.push_back(Ipv4Address(nat->peer2Addresses[0]));
                        EV << "address 0: " << Ipv4Address(nat->peer2Addresses[0]).str() << endl;
                        peerAddressList.push_back(Ipv4Address(nat->peer2Addresses[1]));
                        EV << "address 1: " << Ipv4Address(nat->peer2Addresses[1]).str()  << endl;
                    }
                    else {
                        peerAddress = Ipv4Address(nat->peer2Addresses[0]);
                    }
                    peerPort = nat->portPeer2;
                    delete msg;
                }
                else {
                    auto j = rcvdBytesPerAssoc.find(id);
                    if (j == rcvdBytesPerAssoc.end() && (clientSocket.getState() == SctpSocket::CONNECTED))
                        clientSocket.processMessage(PK(msg));
                    else if (j != rcvdBytesPerAssoc.end()) {
                        j->second += PK(msg)->getBitLength() / 8;
                        auto k = bytesPerAssoc.find(id);
                        k->second->record(j->second);
                        packetsRcvd++;
                        if (!echo) {
                            if ((int64)(long)par("numPacketsToReceivePerClient") > 0) {
                                auto i = rcvdPacketsPerAssoc.find(id);
                                i->second--;
                                SctpSimpleMessage *smsg = check_and_cast<SctpSimpleMessage *>(msg);
                                auto j = endToEndDelay.find(id);
                                j->second->record(simTime() - smsg->getCreationTime());
                                auto k = histEndToEndDelay.find(id);
                                k->second->collect(simTime() - smsg->getCreationTime());

                                if (i->second == 0) {
                                    Request *cmsg = new Request("SCTP_C_NO_OUTSTANDING");
                                    auto& tags = getTags(cmsg);
                                    SctpCommandReq *qinfo = tags.addTagIfAbsent<SctpCommandReq>();
                                    cmsg->setKind(SCTP_C_NO_OUTSTANDING);
                                    qinfo->setSocketId(id);
                                    sendOrSchedule(cmsg);
                                }
                            }
                            delete msg;
                        }
                        else {
                            SctpSimpleMessage *smsg = check_and_cast<SctpSimpleMessage *>(msg->dup());
                            auto j = endToEndDelay.find(id);
                            j->second->record(simTime() - smsg->getCreationTime());
                            auto k = histEndToEndDelay.find(id);
                            k->second->collect(simTime() - smsg->getCreationTime());
                            Packet *cmsg = new Packet("SCTP_C_SEND");
                            bytesSent += smsg->getByteLength();
                            auto cmd = cmsg->addTagIfAbsent<SctpSendReq>();
                            cmd->setSendUnordered(cmd->getSendUnordered());
                            lastStream = (lastStream + 1) % outboundStreams;
                            cmd->setSocketId(id);
                            cmd->setPrValue(0);
                            cmd->setSid(lastStream);
                            cmd->setLast(true);
                            cmsg->encapsulate(smsg);
                            cmsg->setKind(SCTP_C_SEND);
                            cmsg->setControlInfo(cmd);
                            packetsSent++;
                            delete msg;
                            sendOrSchedule(cmsg);
                        }
                    } else {
                        delete msg;
                    }
                }
                break;
            }

            case SCTP_I_SHUTDOWN_RECEIVED: {
                Message *message = check_and_cast<Message *>(msg);
                id = message->getTag<SocketInd>()->getSocketId();
                EV << "peer: SCTP_I_SHUTDOWN_RECEIVED for assoc " << id << "\n";
                auto i = rcvdPacketsPerAssoc.find(id);
                if (i == rcvdPacketsPerAssoc.end() && (clientSocket.getState() == SctpSocket::CONNECTED)) {
                    clientSocket.processMessage(msg);
                } else if (i != rcvdPacketsPerAssoc.end()) {
                    if (i->second == 0) {
                        Request *cmsg = new Request("SCTP_C_NO_OUTSTANDING");
                        auto& tags = getTags(cmsg);
                        SctpCommandReq *qinfo = tags.addTagIfAbsent<SctpCommandReq>();
                        cmsg->setKind(SCTP_C_NO_OUTSTANDING);
                        qinfo->setSocketId(id);
                        sendOrSchedule(cmsg);
                    }

                    shutdownReceived = true;
                    delete msg;
                } else {
                    delete msg;
                }
                delete msg;
            }

            case SCTP_I_SEND_STREAMS_RESETTED:
            case SCTP_I_RCV_STREAMS_RESETTED: {
                EV << "Streams have been resetted\n";
                break;
            }

            case SCTP_I_CLOSED:
            case SCTP_I_SENDSOCKETOPTIONS:
                delete msg;
                break;
        }
    }

    if (hasGUI()) {
        char buf[32];
        auto l = rcvdBytesPerAssoc.find(id);
        sprintf(buf, "rcvd: %lld bytes\nsent: %lld bytes", (long long int)l->second, (long long int)bytesSent);
        getDisplayString().setTagArg("t", 0, buf);
    }
}

void SctpNatPeer::handleTimer(cMessage *msg)
{
    int32 id;

    EV << "SctpNatPeer::handleTimer\n";

    switch (msg->getKind()) {
        case MSGKIND_CONNECT:
            EV << "starting session call connect\n";
            connect(L3AddressResolver().resolve(par("connectAddress"), 1), par("connectPort"));
            delete msg;
            break;

        case SCTP_C_SEND:

            EV << "SctpNatPeer:MSGKIND_SEND\n";

            if (numRequestsToSend > 0) {
                generateAndSend();
                if ((simtime_t)par("thinkTime") > 0)
                    scheduleAt(simTime() + (simtime_t)par("thinkTime"), timeoutMsg);
                numRequestsToSend--;
            }
            break;

        case SCTP_I_ABORT: {
                EV << "SctpNatPeer:MsgKIND_ABORT for assoc " << atoi(msg->getName()) << "\n";

                Request *cmsg = new Request("SCTP_C_CLOSE", SCTP_C_CLOSE);
                auto& tags = getTags(cmsg);
                SctpCommandReq *cmd = tags.addTagIfAbsent<SctpCommandReq>();
                id = atoi(msg->getName());
                cmd->setSocketId(id);
                cmsg->setControlInfo(cmd);
                sendOrSchedule(cmsg);
            }
            break;

        case SCTP_C_RECEIVE:

            EV << "SctpNatPeer:SCTP_C_RECEIVE\n";
            schedule = true;
            sendOrSchedule(msg);
            break;

        default:

            EV << "MsgKind =" << msg->getKind() << " unknown\n";

            break;
    }
}

void SctpNatPeer::socketDataNotificationArrived(int32 connId, void *ptr, Message *msg)
{
    Message *message = check_and_cast<Message *>(msg);
    auto& intags = getTags(message);
    SctpCommandReq *ind = intags.findTag<SctpCommandReq>();
    Request *cmesg = new Request("SCTP_C_RECEIVE");
    auto& outtags = getTags(cmesg);
    SctpSendReq *cmd = outtags.addTagIfAbsent<SctpSendReq>();
    cmd->setSocketId(ind->getSocketId());
    cmd->setSid(ind->getSid());
    cmd->setNumMsgs(ind->getNumMsgs());
    cmesg->setKind(SCTP_C_RECEIVE);
    clientSocket.sendNotification(cmesg);
}

void SctpNatPeer::socketPeerClosed(int32, void *)
{
    // close the connection (if not already closed)
    if (clientSocket.getState() == SctpSocket::PEER_CLOSED) {
        EV << "remote Sctp closed, closing here as well\n";
        setStatusString("closed");
        clientSocket.close();
        if (rendezvous) {
            const char *addressesString = par("localAddress");
            AddressVector addresses = L3AddressResolver().resolve(cStringTokenizer(addressesString).asVector());
            int32 port = par("localPort");
            rendezvousSocket.setOutputGate(gate("socketOut"));
            rendezvousSocket.setOutboundStreams(outboundStreams);
            rendezvousSocket.setInboundStreams(inboundStreams);
            if (addresses.size() == 0) {
                rendezvousSocket.bind(port);
                clientSocket.bind(port);
            }
            else {
                clientSocket.bindx(addresses, port);
                rendezvousSocket.bindx(addresses, port);
            }
            rendezvousSocket.listen(true, (bool)par("streamReset"), par("numPacketsToSendPerClient"));
            if ((bool)par("multi"))
                connectx(peerAddressList, peerPort);
            else
                connect(peerAddress, peerPort);
            rendezvous = false;
        }
    }
}

void SctpNatPeer::socketClosed(int32, void *)
{
    // *redefine* to start another session etc.

    EV << "connection closed\n";
    setStatusString("closed");
    clientSocket.close();
    if (rendezvous) {
        const char *addressesString = par("localAddress");
        AddressVector addresses = L3AddressResolver().resolve(cStringTokenizer(addressesString).asVector());
        int32 port = par("localPort");
        rendezvousSocket.setOutputGate(gate("socketOut"));
        rendezvousSocket.setOutboundStreams(outboundStreams);
        rendezvousSocket.setInboundStreams(inboundStreams);
        if (addresses.size() == 0) {
            rendezvousSocket.bind(port);
            clientSocket.bind(port);
        }
        else {
            clientSocket.bindx(addresses, port);
            rendezvousSocket.bindx(addresses, port);
        }
        rendezvousSocket.listen(true, (bool)par("streamReset"), par("numPacketsToSendPerClient"));
        if ((bool)par("multi"))
            connectx(peerAddressList, peerPort);
        else
            connect(peerAddress, peerPort);
        rendezvous = false;
    }
}

void SctpNatPeer::socketFailure(int32, void *, int32 code)
{
    // subclasses may override this function, and add code try to reconnect after a delay.
    EV << "connection broken\n";
    setStatusString("broken");

    //numBroken++;

    // reconnect after a delay
    timeMsg->setKind(MSGKIND_CONNECT);
    scheduleAt(simTime() + (simtime_t)par("reconnectInterval"), timeMsg);
}

void SctpNatPeer::socketStatusArrived(int assocId, void *yourPtr, SctpStatusReq *status)
{
    struct pathStatus ps;
    auto i = sctpPathStatus.find(status->getPathId());
    if (i != sctpPathStatus.end()) {
        ps = i->second;
        ps.active = status->getActive();
    }
    else {
        ps.active = status->getActive();
        //ps.pid = status->pathId();  FIXME
        ps.primaryPath = false;
        sctpPathStatus[ps.pid] = ps;
    }
    delete status;
}

void SctpNatPeer::setStatusString(const char *s)
{
    if (hasGUI())
        getDisplayString().setTagArg("t", 0, s);
}

void SctpNatPeer::sendRequest(bool last)
{
    EV << "sending request, " << numRequestsToSend - 1 << " more to go\n";
    int64 numBytes = (int64)(long)par("requestLength");
    if (numBytes < 1)
        numBytes = 1;

    EV << "SctpNatPeer: sending " << numBytes << " data bytes\n";

    auto cmsg = new Packet("ApplicationPacket");
    auto msg = makeShared<BytesChunk>();
    std::vector<uint8_t> vec;
    vec.resize(numBytes);
    for (int i = 0; i < numBytes; i++)
        vec[i] = (bytesSent + i) & 0xFF;
    msg->setBytes(vec);
    cmsg->insertAtBack(msg);
    auto creationTimeTag = cmsg->addTagIfAbsent<CreationTimeTag>();
    creationTimeTag->setCreationTime(simTime());
    cmsg->setKind(ordered ? SCTP_C_SEND_ORDERED : SCTP_C_SEND_UNORDERED);
    auto sendCommand = cmsg->addTagIfAbsent<SctpSendReq>();
    sendCommand->setLast(true);
    // send SctpMessage with SctpSimpleMessage enclosed
    clientSocket.sendMsg(cmsg);
    bytesSent += numBytes;
}

void SctpNatPeer::socketEstablished(int32, void *, unsigned long int buffer)
{
    int32 count = 0;
    // *redefine* to perform or schedule first sending
    EV << "SctpNatPeer: socketEstablished\n";
    setStatusString("connected");
    if (rendezvous) {
        uint8_t buffer[100];
        int buflen = 14;
        struct nat_message *nat = (struct nat_message *)(buffer);

        nat->peer1 = par("ownName");
        nat->peer2 = par("peerName");
        nat->portPeer1 = par("localPort");
        nat->portPeer2 = 0;
        nat->peer1Addresses[0] = L3Address().toIpv4().getInt();
        nat->peer2Addresses[0] = L3Address().toIpv4().getInt();
        nat->numAddrPeer1 = 1;
        nat->numAddrPeer2 = 1;
        buflen = ADD_PADDING(buflen + 4 * (nat->numAddrPeer1 +  nat->numAddrPeer2));
        bool mul = par("multi");
        if (mul == true) {
            nat->multi = 1;
        } else {
            nat->multi = 0;
        }

        auto applicationData = makeShared<BytesChunk>(buffer, buflen);
        auto applicationPacket = new Packet("ApplicationPacket");
        applicationPacket->insertAtBack(applicationData);
        auto sctpSendReq = applicationPacket->addTagIfAbsent<SctpSendReq>();
        sctpSendReq->setLast(true);
        sctpSendReq->setPrMethod(0);
        sctpSendReq->setPrValue(0);
        sctpSendReq->setSid(0);
        auto creationTimeTag = applicationPacket->addTagIfAbsent<CreationTimeTag>();
        creationTimeTag->setCreationTime(simTime());
        applicationPacket->setKind(SCTP_C_SEND_ORDERED);
        clientSocket.sendMsg(applicationPacket);

        if ((bool)par("multi")) {
            Request *cmesg = new Request("SCTP_C_SEND_ASCONF");
            auto& tags = getTags(cmesg);
            SctpCommandReq *cmd = tags.addTagIfAbsent<SctpCommandReq>();
            cmd->setSocketId(clientSocket.getConnectionId());
            cmesg->setKind(SCTP_C_SEND_ASCONF);
            clientSocket.sendNotification(cmesg);
        }
    }
    else {
        EV << " determine number of requests in this session\n";
        numRequestsToSend = (int64)(long)par("numRequestsPerSession");
        numPacketsToReceive = (int64)(long)par("numPacketsToReceive");
        if (numRequestsToSend < 1)
            numRequestsToSend = 0;
        // perform first request (next one will be sent when reply arrives)
        if (numRequestsToSend > 0) {
            if ((simtime_t)par("thinkTime") > 0) {
                if (sendAllowed) {
                    sendRequest();
                    numRequestsToSend--;
                }
                timeMsg->setKind(MSGKIND_SEND);
                scheduleAt(simTime() + (simtime_t)par("thinkTime"), timeMsg);
            }
            else {
                if (queueSize > 0) {
                    while (numRequestsToSend > 0 && count++ < queueSize * 2 && sendAllowed) {
                        if (count == queueSize * 2)
                            sendRequest();
                        else
                            sendRequest(false);
                        numRequestsToSend--;
                    }
                    if (numRequestsToSend > 0 && sendAllowed)
                        sendQueueRequest();
                }
                else {
                    while (numRequestsToSend > 0 && sendAllowed) {
                        sendRequest();
                        numRequestsToSend--;
                    }
                }

                if (numPacketsToReceive == 0 && (simtime_t)par("waitToClose") > 0) {
                    timeMsg->setKind(MSGKIND_ABORT);
                    scheduleAt(simTime() + (simtime_t)par("waitToClose"), timeMsg);
                }
                if (numRequestsToSend == 0 && (simtime_t)par("waitToClose") == 0) {
                    EV << "socketEstablished:no more packets to send, call shutdown\n";
                    clientSocket.shutdown();
                }
            }
        }
    }
}

void SctpNatPeer::sendQueueRequest()
{
    Request *cmsg = new Request("SCTP_C_QUEUE_MSGS_LIMIT");
    auto& tags = getTags(cmsg);
    SctpInfoReq *qinfo = tags.addTagIfAbsent<SctpInfoReq>();
    qinfo->setText(queueSize);
    cmsg->setKind(SCTP_C_QUEUE_MSGS_LIMIT);
    qinfo->setSocketId(clientSocket.getConnectionId());
    clientSocket.sendRequest(cmsg);
}

void SctpNatPeer::sendRequestArrived()
{
    int32 count = 0;

    EV << "sendRequestArrived numRequestsToSend=" << numRequestsToSend << "\n";
    while (numRequestsToSend > 0 && count++ < queueSize && sendAllowed) {
        numRequestsToSend--;
        if (count == queueSize || numRequestsToSend == 0)
            sendRequest();
        else
            sendRequest(false);

        if (numRequestsToSend == 0) {
            EV << "no more packets to send, call shutdown\n";
            clientSocket.shutdown();
        }
    }
}

void SctpNatPeer::socketDataArrived(int32, void *, Packet *msg, bool)
{
    // *redefine* to perform or schedule next sending
    packetsRcvd++;

    EV << "Client received packet Nr " << packetsRcvd << " from SCTP\n";

    auto& tags = getTags(msg);
    SctpRcvReq *ind = tags.findTag<SctpRcvReq>();

    bytesRcvd += msg->getByteLength();

    if (echo) {
        const auto& smsg = staticPtrCast<const BytesChunk>(msg->peekData());
        auto creationTimeTag = msg->findTag<CreationTimeTag>();
        creationTimeTag->setCreationTime(simTime());
        auto cmsg = new Packet("ApplicationPacket");
        cmsg->insertAtBack(smsg);
        auto cmd = cmsg->addTagIfAbsent<SctpSendReq>();
        cmd->setLast(true);
        cmd->setSocketId(ind->getSocketId());
        cmd->setPrValue(0);
        cmd->setSid(ind->getSid());
        cmsg->setKind(ind->getSendUnordered() ? SCTP_C_SEND_UNORDERED : SCTP_C_SEND_ORDERED);
        packetsSent++;
        clientSocket.sendMsg(cmsg);
    }
    if ((int64)(long)par("numPacketsToReceive") > 0) {
        numPacketsToReceive--;
        delete msg;
        if (numPacketsToReceive == 0) {
            EV << "Peer: all packets received\n";
        }
    }
}

void SctpNatPeer::shutdownReceivedArrived(int32 connId)
{
    if (numRequestsToSend == 0 || rendezvous) {
        Message *cmsg = new Message("SCTP_C_NO_OUTSTANDING");
        auto& tags = getTags(cmsg);
        SctpCommandReq *qinfo = tags.addTagIfAbsent<SctpCommandReq>();
        cmsg->setKind(SCTP_C_NO_OUTSTANDING);
        qinfo->setSocketId(connId);
        clientSocket.sendNotification(cmsg);
    }
}

void SctpNatPeer::msgAbandonedArrived(int32 assocId)
{
    chunksAbandoned++;
}

void SctpNatPeer::sendqueueFullArrived(int32 assocId)
{
    sendAllowed = false;
}

void SctpNatPeer::addressAddedArrived(int32 assocId, L3Address localAddr, L3Address remoteAddr)
{
    EV << getFullPath() << ": addressAddedArrived for remoteAddr " << remoteAddr << "\n";
    localAddressList.push_back(localAddr);
    clientSocket.addAddress(localAddr);
    if (rendezvous) {
        uint8_t buffer[100];
        int buflen = 16;
        struct nat_message *nat = (struct nat_message *)(buffer);
        nat->peer1 = par("ownName");
        nat->peer2 = par("peerName");
        nat->portPeer1 = par("localPort");
        nat->portPeer2 = 0;
        nat->numAddrPeer1 = 2;
        nat->numAddrPeer1 = 2;
        bool mul = par("multi");
        if (mul == true) {
            nat->multi = 1;
        } else {
            nat->multi = 0;
        }
        nat->peer1Addresses[0] = L3Address().toIpv4().getInt();
        nat->peer2Addresses[0] = L3Address().toIpv4().getInt();
        nat->peer1Addresses[1] = L3Address().toIpv4().getInt();
        nat->peer2Addresses[1] = L3Address().toIpv4().getInt();
        buflen = ADD_PADDING(buflen + 4 * (nat->numAddrPeer1 + nat->numAddrPeer2));

        auto applicationData = makeShared<BytesChunk>(buffer, buflen);
        auto applicationPacket = new Packet("ApplicationPacket");
        applicationPacket->insertAtBack(applicationData);
        auto sctpSendReq = applicationPacket->addTagIfAbsent<SctpSendReq>();
        sctpSendReq->setLast(true);
        sctpSendReq->setPrMethod(0);
        sctpSendReq->setPrValue(0);
        sctpSendReq->setSid(0);
        auto creationTimeTag = applicationPacket->addTagIfAbsent<CreationTimeTag>();
        creationTimeTag->setCreationTime(simTime());
        applicationPacket->setKind(SCTP_C_SEND_ORDERED);
        clientSocket.sendMsg(applicationPacket);
    }
}

void SctpNatPeer::finish()
{
    EV << getFullPath() << ": opened " << numSessions << " sessions\n";
    EV << getFullPath() << ": sent " << bytesSent << " bytes in " << packetsSent << " packets\n";
    for (auto & elem : rcvdBytesPerAssoc) {
        EV << getFullPath() << ": received " << elem.second << " bytes in assoc " << elem.first << "\n";
    }
    EV << getFullPath() << "Over all " << packetsRcvd << " packets received\n ";
    EV << getFullPath() << "Over all " << notifications << " notifications received\n ";
    for (auto & elem : bytesPerAssoc) {
        delete elem.second;
    }
    bytesPerAssoc.clear();
    for (auto & elem : endToEndDelay) {
        delete elem.second;
    }
    endToEndDelay.clear();
    for (auto & elem : histEndToEndDelay) {
        delete elem.second;
    }
    histEndToEndDelay.clear();
    rcvdPacketsPerAssoc.clear();
    sentPacketsPerAssoc.clear();
    rcvdBytesPerAssoc.clear();
}

} // namespace inet
