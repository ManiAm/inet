//
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


#include <omnetpp.h>
#include "PingApp.h"
#include "IPInterfacePacket.h"
#include "IPDatagram.h"
#include "ICMP.h"


/* Ping Application
    doesn't do anything so far */
Define_Module( PingApp );

void PingApp::initialize()
{
    strcpy(nodename, par("nodename"));
}

void PingApp::activity()
{
    cMessage *msg;
    while(true)
    {
        msg = receive();
        ev << "\n*** " << nodename
            << " Ping Application: message received.";
        delete( msg );
    }
}


