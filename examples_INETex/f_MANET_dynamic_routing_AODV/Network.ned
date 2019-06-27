
package inet.examples_INETex.f_MANET_dynamic_routing_AODV;

import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.visualizer.linklayer.DataLinkVisualizer;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.node.inet.AdhocHost;


network Network
{
    @display("bgb=660,630");
    submodules:
        visualizer: IntegratedCanvasVisualizer {
            @display("p=92.564,181.324");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=92.564,35.504");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=91.296,103.976");
        }
        source: AdhocHost {
            @display("p=229,46");
        }
        destination: AdhocHost {
            @display("p=488.832,512.952");
        }
        adhoc1: AdhocHost {
            @display("p=421.296,295.872");
        }
        adhoc2: AdhocHost {
            @display("p=273.36002,440.592");
        }
        adhoc3: AdhocHost {
            @display("p=305.52,188.136");
        }
        adhoc4: AdhocHost {
            @display("p=562.8,328.032");
        }
        adhoc5: AdhocHost {
            @display("p=421.296,77.184");
        }
}


import inet.node.aodv.AodvRouter;
import inet.common.scenario.ScenarioManager;

network Network2
{
    @display("bgb=659.36,730.368");
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
        source: AodvRouter {
            @display("p=229.00801,68.996");
        }
        destination: AodvRouter {
            @display("p=488.84402,535.82");
        }
        aodvRouter1: AodvRouter {
            @display("p=421.316,318.556");
        }
        aodvRouter2: AodvRouter {
            @display("p=274.516,463.888");
        }
        aodvRouter3: AodvRouter {
            @display("p=306.812,211.392");
        }
        aodvRouter4: AodvRouter {
            @display("p=563.71204,350.85202");
        }
        aodvRouter5: AodvRouter {
            @display("p=421.316,99.824005");
        }
}


network Network3
{
    @display("bgb=599.764,494.52");
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
        configurator: Ipv4NetworkConfigurator {
            @display("p=100.172,45.648");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=97.636,125.532");
        }
        source: AodvRouter {
            @display("p=229,46");
        }
        destination: AodvRouter {
            @display("p=505.932,419.708");
        }
        aodvRouter2: AodvRouter {
            @display("p=434.924,323.34");
        }
        aodvRouter1: AodvRouter {
            @display("p=361.38,229.508");
        }
        aodvRouter3: AodvRouter {
            @display("p=386.74,102.708");
        }
}
