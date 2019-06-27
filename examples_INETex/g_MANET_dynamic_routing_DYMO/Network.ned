
package inet.examples_INETex.g_MANET_dynamic_routing_DYMO;

import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.visualizer.linklayer.DataLinkVisualizer;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.node.dymo.DymoRouter;
import inet.common.scenario.ScenarioManager;

network Network
{
    @display("bgb=716.38403,729.596");
    submodules:
        visualizer: IntegratedCanvasVisualizer {
            @display("p=100.172,205.416");
        }
        rreqVisualizer: DataLinkVisualizer {
            @display("p=98.904,285.3");
        }
        rrepVisualizer: DataLinkVisualizer {
            @display("p=98.904,366.452");
        }
        rerrVisualizer: DataLinkVisualizer {
            @display("p=100.172,439.996");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=100.172,45.648");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=97.636,125.532");
        }
        scenarioManager: ScenarioManager {
            @display("p=100.172,524.952");
        }
        source: DymoRouter {
            @display("p=286.26,102.76");
        }
        destination: DymoRouter {
            @display("p=546.096,569.584");
        }
        dymoRouter1: DymoRouter {
            @display("p=478.56802,352.32");
        }
        dymoRouter2: DymoRouter {
            @display("p=330.30002,497.652");
        }
        dymoRouter3: DymoRouter {
            @display("p=364.064,245.156");
        }
        dymoRouter4: DymoRouter {
            @display("p=620.964,384.616");
        }
        dymoRouter5: DymoRouter {
            @display("p=478.56802,133.588");
        }
}

