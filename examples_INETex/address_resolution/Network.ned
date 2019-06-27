
package inet.examples_INETex.address_resolution;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.common.scenario.ScenarioManager;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth10M;
import ned.DatarateChannel;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;

network ARP
{
    @display("bgb=1448.8259,941.602");

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
            @display("p=183.464,377.72");
        }
        hostA1: StandardHost {
            @display("p=183.464,558.486");
        }
        hostA2: StandardHost {
            @display("p=183.464,744.648");
        }
        R1: Router {
            @display("p=661.01,558.486");
        }
        hostB0: StandardHost {
            @display("p=1281.5499,331.854");
        }
        hostB1: StandardHost {
            @display("p=1281.5499,558.486");
        }
        hostB2: StandardHost {
            @display("p=1281.5499,777.024");
        }
        switch0: EtherSwitch {
            @display("p=402.00198,555.78796");
        }
        switch1: EtherSwitch {
            @display("p=971.27997,555.78796");
        }
        
    connections:
        hostA2.ethg++ <--> Eth100M <--> switch0.ethg++;
        hostA1.ethg++ <--> Eth100M <--> switch0.ethg++;
        hostA0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch0.ethg++ <--> Eth100M <--> R1.ethg++;
        switch1.ethg++ <--> Eth100M <--> hostB0.ethg++;
        switch1.ethg++ <--> Eth100M <--> hostB1.ethg++;
        switch1.ethg++ <--> Eth100M <--> hostB2.ethg++;
        switch1.ethg++ <--> Eth100M <--> R1.ethg++;
}
