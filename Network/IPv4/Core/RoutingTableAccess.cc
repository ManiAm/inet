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


#include "RoutingTableAccess.h"

RoutingTable *RoutingTableAccess::get()
{
    if (rt)
        return rt;

    // find Routing Table
    cModule *rtmod = NULL;
    for (cModule *curmod=simulation.contextModule()->parentModule(); !rtmod && curmod; curmod=curmod->parentModule())
        rtmod = curmod->submodule("routingTable");
    if (!rtmod)
        throw new cException("routing table module not found");
    rt = check_and_cast<RoutingTable *>(rtmod);
    return rt;
}

