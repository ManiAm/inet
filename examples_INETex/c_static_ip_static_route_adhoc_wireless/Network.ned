
package inet.examples_INETex.c_static_ip_static_route_adhoc_wireless;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.AdhocHost;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;

network Network
{
    @display("bgb=932.49,528.83997");
    submodules:
        host1: AdhocHost {
            @display("p=329.94,65.52");
        }
        host2: AdhocHost {
            @display("p=548.73,209.43");
        }
        host3: AdhocHost {
            @display("p=604.89,335.79");
        }
        host4: AdhocHost {
            @display("p=395.46,173.16;b=7,5");
        }
        host5: AdhocHost {
            @display("p=740.61,427.05");
        }
        host6: AdhocHost {
            @display("p=451.62,285.48");
        }
        host7: AdhocHost {
            @display("p=869.31,452.79");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=75.79,66.462");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=74.624,137.588");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=75.79,209.88");
        }
}



