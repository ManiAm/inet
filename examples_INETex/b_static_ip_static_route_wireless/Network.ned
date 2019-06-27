
package inet.examples_INETex.b_static_ip_static_route_wireless;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth10M;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.node.inet.WirelessHost;
import inet.node.wireless.AccessPoint;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;


network Network
{
    @display("bgb=1076.4,616.2");
    submodules:
        router0: Router {
            @display("p=322.4,462.8");
        }
        host0: WirelessHost {
            @display("p=88.4,243.09999");
        }
        AP1: AccessPoint {
            @display("p=100.1,360.1");
        }
        host1: WirelessHost {
            @display("p=260,243.09999");
        }
        host6: StandardHost {
            @display("p=963.3,269.1");
        }
        host7: StandardHost {
            @display("p=1014,388.69998");
        }
        host8: StandardHost {
            @display("p=1024.4,539.5");
        }
        router2: Router {
            @display("p=640.89996,462.8");
        }
        switch: EtherSwitch {
            @display("p=830.7,458.9");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=100,50");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=234,49.399998");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=98.799995,115.7");
        }
        router1: Router {
            @display("p=487.5,275.6");
        }
        AP2: AccessPoint {
            @display("p=614.89996,158.59999");
        }
        host3: WirelessHost {
            @display("p=690.3,58.5");
        }
        host4: WirelessHost {
            @display("p=789.1,144.3");
        }
        host2: WirelessHost {
            @display("p=510.9,58.5");
        }
        host5: StandardHost {
            @display("p=741,258.69998");
        }
    connections:
        host8.ethg++ <--> Eth10M <--> switch.ethg++;
        host7.ethg++ <--> Eth10M <--> switch.ethg++;
        switch.ethg++ <--> Eth10M <--> host6.ethg++;
        switch.ethg++ <--> Eth10M <--> router2.ethg++;
        router2.ethg++ <--> Eth10M <--> router0.ethg++;

        router0.ethg++ <--> Eth10M <--> AP1.ethg++;
        router0.ethg++ <--> Eth10M <--> router1.ethg++;

        router1.ethg++ <--> Eth10M <--> router2.ethg++;
        router1.ethg++ <--> Eth10M <--> AP2.ethg++;
        host5.ethg++ <--> Eth10M <--> AP2.ethg++;
}
