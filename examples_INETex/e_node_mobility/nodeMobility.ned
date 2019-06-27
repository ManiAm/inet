
package inet.examples_INETex.e_node_mobility;

import inet.node.inet.StandardHost;
import inet.node.inet.AdhocHost;
import inet.visualizer.integrated.IntegratedVisualizer;


network nodeMobility
{
    parameters:
        @display("bgb=800,600");
        int numHosts;
        
    submodules:
        visualizer: IntegratedVisualizer {
            parameters:
                @display("p=100,50");
        }
        host[numHosts]: StandardHost {
                @display("p=538,158;i=old/ball2");
        }
}
