
package inet.examples_INETex.gettingStarted.example5;

import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.StandardHost;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;

network Network
{
    @display("bgb=1591.1562,473.84375");
    submodules:
        visualizer: IntegratedCanvasVisualizer {
            @display("p=95.875,66.375");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=95.875,167.78125");
        }
        host0: StandardHost {
            @display("p=265.5,66.375");
        }
        host1: StandardHost {
            @display("p=265.5,224.9375");
        }
        host2: StandardHost {
            @display("p=265.5,389.03125");
        }
        host3: StandardHost {
            @display("p=1428.9062,66.375");
        }
        host4: StandardHost {
            @display("p=1428.9062,228.625");
        }
        host5: StandardHost {
            @display("p=1428.9062,389.03125");
        }
        router0: Router {
            @display("p=667.4375,224.9375");
        }
        router1: Router {
            @display("p=971.65625,224.9375");
        }
        switch0: EtherSwitch {
            @display("p=490.4375,224.9375");
        }
        switch1: EtherSwitch {
            @display("p=1178.1562,224.9375");
        }
    connections:
        switch0.ethg++ <--> Eth100M <--> router0.ethg++;
        router1.ethg++ <--> Eth100M <--> switch1.ethg++;
        router0.ethg++ <--> Eth100M <--> router1.ethg++;
        host0.ethg++ <--> Eth100M <--> switch0.ethg++;
        host2.ethg++ <--> Eth100M <--> switch0.ethg++;
        host1.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch1.ethg++ <--> Eth100M <--> host3.ethg++;
        switch1.ethg++ <--> Eth100M <--> host4.ethg++;
        switch1.ethg++ <--> Eth100M <--> host5.ethg++;
}

