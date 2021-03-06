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

#ifndef __INET_PACKETCOLLECTOR_H
#define __INET_PACKETCOLLECTOR_H

#include "inet/common/queueing/base/PacketSinkBase.h"
#include "inet/common/queueing/contract/IPacketCollector.h"

namespace inet {
namespace queueing {

class INET_API PacketCollector : public PacketSinkBase, public IPacketCollector
{
  public:
    cGate *inputGate = nullptr;
    IPacketProvider *provider = nullptr;

    cPar *collectionIntervalParameter = nullptr;
    cMessage *collectionTimer = nullptr;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *message) override;

    virtual void scheduleCollectionTimer();
    virtual void collectPacket();

  public:
    virtual ~PacketCollector() { cancelAndDelete(collectionTimer); }

    virtual IPacketProvider *getProvider(cGate *gate) override { return provider; }

    virtual bool supportsPushPacket(cGate *gate) override { return false; }
    virtual bool supportsPopPacket(cGate *gate) override { return inputGate == gate; }

    virtual void handleCanPopPacket(cGate *gate) override;
};

} // namespace queueing
} // namespace inet

#endif // ifndef __INET_PACKETCOLLECTOR_H

