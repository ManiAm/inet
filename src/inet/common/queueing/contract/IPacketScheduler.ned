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
// This module interface is implemented by packet scheduler modules. A packet
// scheduler is a passive module which has multiple active inputs and one
// passive output. Packets popped from the passive output are provided by
// one of the inputs without any delay and reordering.
//
moduleinterface IPacketScheduler extends IPacketProvider
{
    parameters:
        @display("i=block/join");
    gates:
        input in[] @labels(pop);
}
