
package inet.examples_INETex.gettingStarted.example4;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.node.ethernet.Eth100M;
import inet.node.inet.AdhocHost;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;


network Network
{
    @display("bgb=1241.2238,296.1225");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=87,57");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=86.7825,144.6375");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=85.26,226.8525");
        }
        host1: AdhocHost {
            @display("p=312.1125,226.8525");
        }
        host2: AdhocHost {
            @display("p=476.01,225.55125");
        }
        ethHost1: StandardHost {
            @display("p=313.635,79.17");
        }
        ethHost2: StandardHost {
            @display("p=1116.6863,78.87375");
        }
        router1: Router {
            @display("p=581.595,79.17");
        }
        router2: Router {
            @display("p=857.925,78.87375");
        }
    connections:
        ethHost1.ethg++ <--> Eth100M <--> router1.ethg++;
        router2.ethg++ <--> Eth100M <--> ethHost2.ethg++;
        router1.pppg++ <--> Eth100M <--> router2.pppg++;
}
