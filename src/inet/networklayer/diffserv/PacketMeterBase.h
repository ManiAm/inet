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

#ifndef __INET_PACKETMETERBASE_H
#define __INET_PACKETMETERBASE_H

#include "inet/common/queueing/base/PacketConsumerBase.h"
#include "inet/common/queueing/contract/IPacketProducer.h"
#include "inet/common/queueing/contract/IPacketQueueingElement.h"

namespace inet {

using namespace inet::queueing;

class INET_API PacketMeterBase : public PacketConsumerBase, public IPacketProducer
{
  protected:
    cGate *inputGate = nullptr;
    IPacketProducer *producer = nullptr;

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual int meterPacket(Packet *packet) = 0;

  public:
    virtual IPacketConsumer *getConsumer(cGate *gate) override { throw cRuntimeError("Invalid operation"); }

    virtual bool supportsPushPacket(cGate *gate) override { return true; }
    virtual bool supportsPopPacket(cGate *gate) override { return false; }

    virtual void handleCanPushPacket(cGate *gate) override;
};

} // namespace inet

#endif // ifndef __INET_PACKETMETERBASE_H

