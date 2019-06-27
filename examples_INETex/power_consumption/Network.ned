package inet.examples_INETex.power_consumption;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.AdhocHost;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;

network Network
{
    parameters:
        int numHost;
        @display("bgb=383,303");
        
    submodules:
        visualizer: IntegratedCanvasVisualizer {
            parameters:
                @display("p=76,32");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=76,98");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            parameters:
                @display("p=75,164");
        }
        host[numHost]: AdhocHost {
            parameters:
                @display("p=314,193");
        }
}
