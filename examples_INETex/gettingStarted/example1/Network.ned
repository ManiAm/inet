
package inet.examples_INETex.gettingStarted.example1;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.common.scenario.ScenarioManager;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth10M;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.node.inet.WirelessHost;
import inet.node.wireless.AccessPoint;

network Network
{
    @display("bgb=1101.88,848.38");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=99.9,29.7");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=273.78,28.73");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=99.9,118.8");
        }
        scenarioManager: ScenarioManager {
            @display("p=275.47,118.299995");
        }
        router0: Router {
            @display("p=321.30002,522.45");
        }
        whost0: WirelessHost {
            @display("p=87.75,303.75");
        }
        AP0: AccessPoint {
            @display("p=99.9,419.85");
        }
        whost1: WirelessHost {
            @display("p=259.2,303.75");
        }
        host7: StandardHost {
            @display("p=1043.9,388.69998");
        }
        host8: StandardHost {
            @display("p=1043.9,521.3");
        }
        host9: StandardHost {
            @display("p=1043.9,651.3");
        }
        router2: Router {
            @display("p=639.9,522.45");
        }
        switch2: EtherSwitch {
            @display("p=829.39996,520");
        }
        router1: Router {
            @display("p=487.35,336.15002");
        }
        AP1: AccessPoint {
            @display("p=614.25,218.70001");
        }
        whost3: WirelessHost {
            @display("p=689.85004,118.8");
        }
        whost4: WirelessHost {
            @display("p=788.4,203.85");
        }
        whost2: WirelessHost {
            @display("p=510.30002,118.8");
        }
        host6: StandardHost {
            @display("p=739.80005,318.6");
        }
        host5: StandardHost {
            @display("p=838.5,774.8");
        }
        host4: StandardHost {
            @display("p=700.674,775.302");
        }
        host3: StandardHost {
            @display("p=536.89996,774.8");
        }
        switch1: EtherSwitch {
            @display("p=700.674,642.63");
        }
        host0: StandardHost {
            @display("p=67.6,774.8");
        }
        host1: StandardHost {
            @display("p=226.2,774.8");
        }
        host2: StandardHost {
            @display("p=361.4,774.8");
        }
        switch0: EtherSwitch {
            @display("p=226.2,642.2");
        }
    connections:
        host9.ethg++ <--> Eth100M <--> switch2.ethg++;
        host8.ethg++ <--> Eth100M <--> switch2.ethg++;
        switch2.ethg++ <--> Eth100M <--> host7.ethg++;
        switch2.ethg++ <--> Eth100M <--> router2.ethg++;
        router2.ethg++ <--> Eth10M <--> router0.ethg++;
        router0.ethg++ <--> Eth100M <--> AP0.ethg++;
        router1.ethg++ <--> Eth100M <--> AP1.ethg++;
        host6.ethg++ <--> Eth100M <--> AP1.ethg++;
        router0.ethg++ <--> Eth10M <--> router1.ethg++;
        router1.ethg++ <--> Eth100M <--> router2.ethg++;
        host3.ethg++ <--> Eth100M <--> switch1.ethg++;
        host4.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch1.ethg++ <--> Eth100M <--> host5.ethg++;
        host0.ethg++ <--> Eth100M <--> switch0.ethg++;
        host1.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch0.ethg++ <--> Eth100M <--> host2.ethg++;
        switch0.ethg++ <--> Eth100M <--> router0.ethg++;
        switch1.ethg++ <--> Eth100M <--> router2.ethg++;
}

