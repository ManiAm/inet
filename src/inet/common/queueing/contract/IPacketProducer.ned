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

package inet.common.queueing.contract;

//
// This module interface is implemented by packet producer modules. A packet
// producer is an active module with one output. Packets are actively pushed
// into the connected module repeatedly over time.
//
moduleinterface IPacketProducer
{
    parameters:
        @display("i=block/source");
    gates:
        output out @labels(push);
}
