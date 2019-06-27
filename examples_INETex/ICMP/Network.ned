
package inet.examples_INETex.ICMP;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.common.scenario.ScenarioManager;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth10M;
import ned.DatarateChannel;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;

network ICMP
{
    @display("bgb=1568.7,803.25");

    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=155.09625,99.8925");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=420.6,99.8925");
        }
        scenarioManager: ScenarioManager {
            @display("p=707.1337,99.8925");
        }
        hostA0: StandardHost {
            @display("p=124.74,302.4");
        }
        hostA1: StandardHost {
            @display("p=124.74,483.84");
        }
        hostA2: StandardHost {
            @display("p=124.74,669.06");
        }
        R1: Router {
            @display("p=601.02,483.84");
        }
        R2: Router {
            @display("p=893.97,483.84");
        }
        hostB0: StandardHost {
            @display("p=1462.86,257.04");
        }
        hostB1: StandardHost {
            @display("p=1462.86,483.84");
        }
        hostB2: StandardHost {
            @display("p=1462.86,703.08");
        }
        switch0: EtherSwitch {
            @display("p=342.09,481.94998");
        }
        switch1: EtherSwitch {
            @display("p=1151.01,481.94998");
        }
    connections:
        hostA2.ethg++ <--> Eth100M <--> switch0.ethg++;
        hostA1.ethg++ <--> Eth100M <--> switch0.ethg++;
        hostA0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch0.ethg++ <--> Eth100M <--> R1.ethg++;
        switch1.ethg++ <--> Eth100M <--> hostB0.ethg++;
        switch1.ethg++ <--> Eth100M <--> hostB1.ethg++;
        switch1.ethg++ <--> Eth100M <--> hostB2.ethg++;
        R1.ethg++ <--> Eth100M <--> R2.ethg++;
        R2.ethg++ <--> Eth100M <--> switch1.ethg++;
}
