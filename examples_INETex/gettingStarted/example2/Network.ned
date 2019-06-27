
package inet.examples_INETex.gettingStarted.example2;

import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.visualizer.integrated.IntegratedMultiVisualizer;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.node.inet.AdhocHost;
import inet.node.aodv.AodvRouter;
import inet.common.scenario.ScenarioManager;

network Network
{
    @display("bgb=613.104,277.296");
    submodules:
        visualizer: IntegratedCanvasVisualizer {
            @display("p=100.172,205.416");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=100.172,45.648");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=97.636,125.532");
        }
        ahost0: AdhocHost {
            @display("p=318,76.32");
        }
        ahost1: AdhocHost {
            @display("p=521.52,76.32");
        }
}

network Network2
{
    @display("bgb=659.132,632.708");
    submodules:
        visualizer: IntegratedMultiVisualizer {
            @display("p=100.172,205.416");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=100.172,45.648");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=97.636,125.532");
        }
        scenarioManager: ScenarioManager {
            @display("p=99.216,292.56");
        }
        source: AodvRouter {
            @display("p=229.00801,68.996");
        }
        destination: AodvRouter {
            @display("p=488.84402,535.82");
        }
        ahost2: AodvRouter {
            @display("p=421.316,318.556");
        }
        ahost4: AodvRouter {
            @display("p=274.516,463.888");
        }
        ahost1: AodvRouter {
            @display("p=306.812,211.392");
        }
        ahost3: AodvRouter {
            @display("p=563.71204,350.85202");
        }
        ahost0: AodvRouter {
            @display("p=421.316,99.824005");
        }
}
