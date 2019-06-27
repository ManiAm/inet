
package inet.examples_INETex.AckingWirelessInterface;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.node.inet.WirelessHost;
import inet.physicallayer.unitdisk.UnitDiskRadioMedium;

network Network
{
    parameters:
        @display("bgb=529,391;bgg=100,1,grey95");
        @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

        @figure[rcvdPkText](type=indicatorText; pos=250,20; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
        @statistic[packetReceived](source=hostB.app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);

    submodules:
        visualizer: IntegratedCanvasVisualizer {
            @display("p=73,20");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=73,74");
        }
        radioMedium: UnitDiskRadioMedium {
            @display("p=72,132");
        }
        hostA: WirelessHost {
            @display("p=50,325");
        }
        hostB: WirelessHost {
            @display("p=450,325");
        }
}
