
package inet.examples_INETex.d_dynamic_ip_DHCP;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.ethernet.Eth10M;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.node.inet.WirelessHost;
import inet.node.wireless.AccessPoint;

network Network
{
    @display("bgb=1075.9501,687.15");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=99.9,29.7");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=257.85,29.7");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=99.9,118.8");
        }
        router0: Router {
            @display("p=321.30002,522.45");
        }
        host0: WirelessHost {
            @display("p=87.75,303.75");
        }
        AP1: AccessPoint {
            @display("p=99.9,419.85");
        }
        host1: WirelessHost {
            @display("p=259.2,303.75");
        }
        host6: StandardHost {
            @display("p=962.55005,329.40002");
        }
        host7: StandardHost {
            @display("p=1013.85004,448.2");
        }
        host8: StandardHost {
            @display("p=1023.30005,599.4");
        }
        router2: Router {
            @display("p=639.9,522.45");
        }
        switch: EtherSwitch {
            @display("p=830.25,518.4");
        }
        router1: Router {
            @display("p=487.35,336.15002");
        }
        AP2: AccessPoint {
            @display("p=614.25,218.70001");
        }
        host3: WirelessHost {
            @display("p=689.85004,118.8");
        }
        host4: WirelessHost {
            @display("p=788.4,203.85");
        }
        host2: WirelessHost {
            @display("p=510.30002,118.8");
        }
        host5: StandardHost {
            @display("p=739.80005,318.6");
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


import inet.common.scenario.ScenarioManager;

network Network2
{
    @display("bgb=692.73,375.9");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=100,50");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=234,49.399998");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=98.799995,115.7");
        }
        scenarioManager: ScenarioManager {
            @display("p=97.734,185.802");
        }
        host0: StandardHost {
            @display("p=591.774,79.476");
        }
        host1: StandardHost {
            @display("p=645.474,198.69");
        }
        host2: StandardHost {
            @display("p=591.774,316.83");
        }
        router0: Router {
            @display("p=257.76,207.282");
        }
        switch: EtherSwitch {
            @display("p=447.858,204.06");
        }
    connections:
        host2.ethg++ <--> Eth10M <--> switch.ethg++;
        host1.ethg++ <--> Eth10M <--> switch.ethg++;
        switch.ethg++ <--> Eth10M <--> host0.ethg++;
        switch.ethg++ <--> Eth10M <--> router0.ethg++;
}
