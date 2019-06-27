
package inet.examples_INETex.wirelessCommunication;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.node.aodv.AodvRouter;

network Network
{
    parameters:
        int numHosts;
        @display("bgb=320,223");

    submodules:
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=55,36");
        }
        visualizer: IntegratedCanvasVisualizer {
            parameters:
                @display("p=50,174");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            parameters:
                @display("p=55,102");
        }
        host[numHosts]: AodvRouter {
            parameters:
                @display("i=device/pocketpc_s;r=,,#707070;p=228,103");
        }
}
