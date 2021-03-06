//
// Copyright (C) OpenSim Ltd.
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
// along with this program; if not, see http://www.gnu.org/licenses/.
//

#ifndef __INET_PACKETBASEDTOKENGENERATOR_H
#define __INET_PACKETBASEDTOKENGENERATOR_H

#include "inet/common/queueing/base/PacketConsumerBase.h"
#include "inet/common/queueing/TokenBasedServer.h"

namespace inet {
namespace queueing {

class INET_API PacketBasedTokenGenerator : public PacketConsumerBase, public cListener
{
  protected:
    int numTokensPerPacket = -1;
    int numTokensPerBit = -1;

    cGate *inputGate = nullptr;
    IPacketProducer *producer = nullptr;
    TokenBasedServer *server = nullptr;

    int numTokensGenerated = -1;

  protected:
    virtual void initialize(int stage) override;

  public:
    virtual bool supportsPushPacket(cGate *gate) override { return true; }
    virtual bool supportsPopPacket(cGate *gate) override { return false; }

    virtual bool canPushSomePacket(cGate *gate) override { return server->getNumTokens() == 0; }
    virtual bool canPushPacket(Packet *packet, cGate *gate) override { return server->getNumTokens() == 0; }
    virtual void pushPacket(Packet *packet, cGate *gate = nullptr) override;

    virtual const char *resolveDirective(char directive) override;
    virtual void receiveSignal(cComponent *source, simsignal_t signal, cObject *object, cObject *details) override;
};

} // namespace queueing
} // namespace inet

#endif // ifndef __INET_PACKETBASEDTOKENGENERATOR_H

