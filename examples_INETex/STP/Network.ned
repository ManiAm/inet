
package inet.examples_INETex.STP;

import inet.linklayer.ethernet.EtherHub;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.linklayer.configurator.L2NetworkConfigurator;
import inet.common.scenario.ScenarioManager;
import inet.node.ethernet.Eth1G;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth10M;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;


network Basic_Network
{
    @display("bgb=1287.2124,536.01245");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=162.98875,101.666245");
        }
        l2NetworkConfigurator: L2NetworkConfigurator {
            @display("p=162.98875,256.58624");
        }
        host0: StandardHost {
            @display("p=381.46872,101.725");
        }
        host1: StandardHost {
            @display("p=381.46872,258.22498");
        }
        host2: StandardHost {
            @display("p=381.46872,420.59372");
        }
        host3: StandardHost {
            @display("p=1154.1875,99.768745");
        }
        host4: StandardHost {
            @display("p=1154.1875,258.22498");
        }
        host5: StandardHost {
            @display("p=1154.1875,418.63748");
        }
        switch0: EtherSwitch {
            @display("p=602.52496,256.26874");
        }
        switch1: EtherSwitch {
            @display("p=903.7875,256.26874");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=162.98875,417.96124");
        }
    connections:
        host2.ethg++ <--> Eth100M <--> switch0.ethg++;
        host1.ethg++ <--> Eth100M <--> switch0.ethg++;
        host0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch1.ethg++ <--> Eth100M <--> host3.ethg++;
        switch1.ethg++ <--> Eth100M <--> host4.ethg++;
        switch1.ethg++ <--> Eth100M <--> host5.ethg++;
        switch1.ethg++ <--> Eth100M <--> switch0.ethg++;
}

network Loop_Network
{
    @display("bgb=1287.2124,536.01245");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=162.98875,69.39125");
        }
        l2NetworkConfigurator: L2NetworkConfigurator {
            @display("p=162.98875,195.26375");
        }
        scenarioManager: ScenarioManager {
            @display("p=162.98875,308.22626");
        }
        host0: StandardHost {
            @display("p=381.46872,101.725");
        }
        host1: StandardHost {
            @display("p=381.46872,258.22498");
        }
        host2: StandardHost {
            @display("p=381.46872,420.59372");
        }
        host3: StandardHost {
            @display("p=1154.1875,99.768745");
        }
        host4: StandardHost {
            @display("p=1154.1875,258.22498");
        }
        host5: StandardHost {
            @display("p=1154.1875,418.63748");
        }
        switch0: EtherSwitch {
            @display("p=602.52496,256.26874");
        }
        switch1: EtherSwitch {
            @display("p=903.7875,256.26874");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=162.98875,429.25748");
        }
        switch2: EtherSwitch {
            @display("p=753.6212,396.98248");
        }
    connections:
        host2.ethg++ <--> Eth100M <--> switch0.ethg++;
        host1.ethg++ <--> Eth100M <--> switch0.ethg++;
        host0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch1.ethg++ <--> Eth100M <--> host3.ethg++;
        switch1.ethg++ <--> Eth100M <--> host4.ethg++;
        switch1.ethg++ <--> Eth100M <--> host5.ethg++;
        switch1.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch2.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch2.ethg++ <--> Eth100M <--> switch1.ethg++;
}

network Loop_Network2
{
    @display("bgb=1287.2124,536.01245");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=162.98875,69.39125");
        }
        l2NetworkConfigurator: L2NetworkConfigurator {
            @display("p=162.98875,195.26375");
        }
        scenarioManager: ScenarioManager {
            @display("p=162.98875,308.22626");
        }
        host0: StandardHost {
            @display("p=381.46872,101.725");
        }
        host1: StandardHost {
            @display("p=381.46872,258.22498");
        }
        host2: StandardHost {
            @display("p=381.46872,420.59372");
        }
        host3: StandardHost {
            @display("p=1154.1875,99.768745");
        }
        host4: StandardHost {
            @display("p=1154.1875,258.22498");
        }
        host5: StandardHost {
            @display("p=1154.1875,418.63748");
        }
        switch0: EtherSwitch {
            @display("p=602.52496,256.26874");
        }
        switch1: EtherSwitch {
            @display("p=903.7875,256.26874");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=162.98875,429.25748");
        }
        switch2: EtherSwitch {
            @display("p=903.7,416.3475");
        }
        switch3: EtherSwitch {
            @display("p=601.9287,416.3475");
        }
    connections:
        host2.ethg++ <--> Eth100M <--> switch0.ethg++;
        host1.ethg++ <--> Eth100M <--> switch0.ethg++;
        host0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch1.ethg++ <--> Eth100M <--> host3.ethg++;
        switch1.ethg++ <--> Eth100M <--> host4.ethg++;
        switch1.ethg++ <--> Eth100M <--> host5.ethg++;
        switch1.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch2.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch3.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch3.ethg++ <--> Eth100M <--> switch2.ethg++;
}

network Loop_Network3
{
    @display("bgb=1287.2124,536.01245");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=162.98875,69.39125");
        }
        l2NetworkConfigurator: L2NetworkConfigurator {
            @display("p=162.98875,195.26375");
        }
        scenarioManager: ScenarioManager {
            @display("p=162.98875,308.22626");
        }
        host0: StandardHost {
            @display("p=381.46872,101.725");
        }
        host1: StandardHost {
            @display("p=381.46872,258.22498");
        }
        host2: StandardHost {
            @display("p=381.46872,420.59372");
        }
        host3: StandardHost {
            @display("p=1154.1875,99.768745");
        }
        host4: StandardHost {
            @display("p=1154.1875,258.22498");
        }
        host5: StandardHost {
            @display("p=1154.1875,418.63748");
        }
        switch0: EtherSwitch {
            @display("p=602.52496,256.26874");
        }
        switch1: EtherSwitch {
            @display("p=903.7875,256.26874");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=162.98875,429.25748");
        }
        switch2: EtherSwitch {
            @display("p=903.7,416.3475");
        }
        switch3: EtherSwitch {
            @display("p=601.9287,416.3475");
        }
    connections:
        host2.ethg++ <--> Eth100M <--> switch0.ethg++;
        host1.ethg++ <--> Eth100M <--> switch0.ethg++;
        host0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch1.ethg++ <--> Eth100M <--> host3.ethg++;
        switch1.ethg++ <--> Eth100M <--> host4.ethg++;
        switch1.ethg++ <--> Eth100M <--> host5.ethg++;
        switch1.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch2.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch3.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch3.ethg++ <--> Eth1G <--> switch2.ethg++;
}

network Loop_Network4
{
    @display("bgb=1287.2124,536.01245");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=162.98875,69.39125");
        }
        l2NetworkConfigurator: L2NetworkConfigurator {
            @display("p=162.98875,195.26375");
        }
        scenarioManager: ScenarioManager {
            @display("p=162.98875,308.22626");
        }
        host0: StandardHost {
            @display("p=381.46872,101.725");
        }
        host1: StandardHost {
            @display("p=381.46872,258.22498");
        }
        host2: StandardHost {
            @display("p=381.46872,420.59372");
        }
        host3: StandardHost {
            @display("p=1154.1875,99.768745");
        }
        host4: StandardHost {
            @display("p=1154.1875,258.22498");
        }
        host5: StandardHost {
            @display("p=1154.1875,418.63748");
        }
        switch0: EtherSwitch {
            @display("p=602.52496,256.26874");
        }
        switch1: EtherSwitch {
            @display("p=903.7875,256.26874");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=162.98875,429.25748");
        }
        switch2: EtherSwitch {
            @display("p=753.6212,396.98248");
        }
        etherHub: EtherHub {
            @display("p=601.9287,377.6175");
        }
    connections:
        host2.ethg++ <--> Eth100M <--> switch0.ethg++;
        host1.ethg++ <--> Eth100M <--> switch0.ethg++;
        host0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch1.ethg++ <--> Eth100M <--> host3.ethg++;
        switch1.ethg++ <--> Eth100M <--> host4.ethg++;
        switch1.ethg++ <--> Eth100M <--> host5.ethg++;
        switch1.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch2.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch2.ethg++ <--> Eth100M <--> etherHub.ethg++;
        etherHub.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch2.ethg++ <--> Eth100M <--> etherHub.ethg++;
}

network Loop_Network5
{
    @display("bgb=1286.1587,918.22375");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=162.98875,69.39125");
        }
        l2NetworkConfigurator: L2NetworkConfigurator {
            @display("p=162.98875,195.26375");
        }
        scenarioManager: ScenarioManager {
            @display("p=162.98875,308.22626");
        }
        host0: StandardHost {
            @display("p=451.85,124.25875");
        }
        host1: StandardHost {
            @display("p=451.85,280.79248");
        }
        host2: StandardHost {
            @display("p=451.85,442.16748");
        }
        host3: StandardHost {
            @display("p=1132.8525,505.10373");
        }
        host4: StandardHost {
            @display("p=1132.8525,663.2512");
        }
        host5: StandardHost {
            @display("p=1132.8525,824.6262");
        }
        switch0: EtherSwitch {
            @display("p=755.235,296.93");
        }
        switch1: EtherSwitch {
            @display("p=900.4725,480.8975");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=162.98875,429.25748");
        }
        switch2: EtherSwitch {
            @display("p=587.40497,653.5687");
        }
        switch3: EtherSwitch {
            @display("p=587.40497,471.215");
        }
        switch4: EtherSwitch {
            @display("p=900.4725,663.2512");
        }
    connections:
        host2.ethg++ <--> Eth100M <--> switch0.ethg++;
        host1.ethg++ <--> Eth100M <--> switch0.ethg++;
        host0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch1.ethg++ <--> Eth10M <--> switch0.ethg++;
        switch2.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch3.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch3.ethg++ <--> Eth1G <--> switch2.ethg++;
        switch4.ethg++ <--> Eth100M <--> switch2.ethg++;
        switch4.ethg++ <--> Eth10M <--> switch1.ethg++;
        host3.ethg++ <--> Eth100M <--> switch4.ethg++;
        host4.ethg++ <--> Eth100M <--> switch4.ethg++;
        host5.ethg++ <--> Eth100M <--> switch4.ethg++;
}

network stp_limitation_1
{
    @display("bgb=1202.2881,831.64404");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=162.98875,69.39125");
        }
        l2NetworkConfigurator: L2NetworkConfigurator {
            @display("p=162.98875,195.26375");
        }
        scenarioManager: ScenarioManager {
            @display("p=162.98875,308.22626");
        }
        host0: StandardHost {
            @display("p=425.96402,86.668");
        }
        host1: StandardHost {
            @display("p=1115.62,442.56003");
        }
        switch0: EtherSwitch {
            @display("p=724.692,84.824005");
        }
        switch1: EtherSwitch {
            @display("p=870.36804,267.38");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=162.98875,429.25748");
        }
        switch2: EtherSwitch {
            @display("p=556.888,440.716");
        }
        switch3: EtherSwitch {
            @display("p=556.888,258.16");
        }
        switch4: EtherSwitch {
            @display("p=870.36804,449.936");
        }
        switch5: EtherSwitch {
            @display("p=556.888,625.116");
        }
        switch6: EtherSwitch {
            @display("p=724.692,756.04004");
        }
        switch7: EtherSwitch {
            @display("p=870.36804,625.116");
        }
    connections:
        host0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch1.ethg++ <--> Eth10M <--> switch0.ethg++;
        switch3.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch3.ethg++ <--> Eth1G <--> switch2.ethg++;
        switch4.ethg++ <--> Eth10M <--> switch1.ethg++;
        host1.ethg++ <--> Eth100M <--> switch4.ethg++;
        switch2.ethg++ <--> Eth100M <--> switch5.ethg++;
        switch5.ethg++ <--> Eth100M <--> switch6.ethg++;
        switch6.ethg++ <--> Eth100M <--> switch7.ethg++;
        switch7.ethg++ <--> Eth100M <--> switch4.ethg++;
}

network stp_limitation_2
{
    @display("bgb=1386.1,716.43");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=162.98875,69.39125");
        }
        l2NetworkConfigurator: L2NetworkConfigurator {
            @display("p=162.98875,195.26375");
        }
        scenarioManager: ScenarioManager {
            @display("p=162.98875,308.22626");
        }
        host0: StandardHost {
            @display("p=425.96402,86.668");
        }
        host1: StandardHost {
            @display("p=1132.26,612.89");
        }
        switch0: EtherSwitch {
            @display("p=724.692,84.824005");
        }
        switch1: EtherSwitch {
            @display("p=870.36804,267.38");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=162.98875,429.25748");
        }
        switch2: EtherSwitch {
            @display("p=983.63,83.5");
        }
        switch3: EtherSwitch {
            @display("p=556.888,258.16");
        }
        switch4: EtherSwitch {
            @display("p=1115.56,267.2");
        }
        switch5: EtherSwitch {
            @display("p=1260.85,95.19");
        }
        switch6: EtherSwitch {
            @display("p=723.11,427.52002");
        }
        switch7: EtherSwitch {
            @display("p=1115.56,439.21");
        }
    connections:
        switch0.ethg++ <--> Eth100M <--> switch3.ethg++;
        switch0.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch1.ethg++ <--> Eth100M <--> switch3.ethg++;
        switch3.ethg++ <--> Eth100M <--> switch6.ethg++;
        switch6.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch1.ethg++ <--> Eth100M <--> switch2.ethg++;
        switch2.ethg++ <--> Eth100M <--> switch4.ethg++;
        switch4.ethg++ <--> Eth100M <--> switch1.ethg++;
        switch4.ethg++ <--> Eth100M <--> switch5.ethg++;
        switch6.ethg++ <--> Eth100M <--> switch7.ethg++;
        switch7.ethg++ <--> Eth100M <--> switch4.ethg++;
}
