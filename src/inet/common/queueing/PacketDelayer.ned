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

package inet.common.queueing;

import inet.common.queueing.contract.IPacketConsumer;
import inet.common.queueing.contract.IPacketProducer;

//
// This module connects one packet producer to one packet consumer. It can be
// pushed with packets from the connected packet producer. When this happens,
// the packet is delayed and eventually pushed to the output. Note that the
// order of packets may change if the delay parameter is configured to be a
// distribution.
//
simple PacketDelayer like IPacketConsumer, IPacketProducer
{
    parameters:
        volatile double delay @unit(s);
        @display("i=block/delay");
    gates:
        input in @labels(push);
        output out @labels(push);
}
