
package inet.examples_INETex.multicasting;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;
import inet.common.scenario.ScenarioManager;
import inet.node.ethernet.Eth100M;
import inet.node.ethernet.Eth10M;
import ned.DatarateChannel;
import inet.node.ethernet.EtherSwitch;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;

network IPv4_Casting
{
    @display("bgb=1766.52,1345.92");

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
            @display("p=199.785,567.81");
        }
        hostA1: StandardHost {
            @display("p=199.785,749.1937");
        }
        hostA2: StandardHost {
            @display("p=199.785,935.835");
        }
        hostB0: StandardHost {
            @display("p=1635.0825,570.4387");
        }
        R1: Router {
            @display("p=678.21747,749.1937");
        }
        R2: Router {
            @display("p=1132.9912,749.1937");
        }
        hostB1: StandardHost {
            @display("p=1635.0825,749.1937");
        }
        hostB2: StandardHost {
            @display("p=1635.0825,951.6075");
        }
        hostC0: StandardHost {
            @display("p=473.175,1209.225");
        }
        hostC1: StandardHost {
            @display("p=678.21747,1209.225");
        }
        hostC2: StandardHost {
            @display("p=893.77496,1209.225");
        }
        switch0: EtherSwitch {
            @display("p=420.6,746.565");
        }
        switch2: EtherSwitch {
            @display("p=678.21747,993.6675");
        }
        switch1: EtherSwitch {
            @display("p=1406.3812,746.565");
        }
        switch3: EtherSwitch {
            @display("p=1132.9912,525.75");
        }
        hostD0: StandardHost {
            @display("p=893.77496,294.41998");
        }
        hostD1: StandardHost {
            @display("p=1127.7338,297.04874");
        }
        hostD2: StandardHost {
            @display("p=1356.4349,291.79126");
        }
    connections:
        hostA2.ethg++ <--> Eth100M <--> switch0.ethg++;
        hostA1.ethg++ <--> Eth100M <--> switch0.ethg++;
        hostA0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch0.ethg++ <--> Eth100M <--> R1.ethg++;

        switch1.ethg++ <--> Eth100M <--> hostB0.ethg++;
        switch1.ethg++ <--> Eth100M <--> hostB1.ethg++;
        switch1.ethg++ <--> Eth100M <--> hostB2.ethg++;
        switch2.ethg++ <--> Eth100M <--> hostC0.ethg++;
        switch2.ethg++ <--> Eth100M <--> hostC1.ethg++;
        switch2.ethg++ <--> Eth100M <--> hostC2.ethg++;

        R1.ethg++ <--> Eth100M <--> R2.ethg++;
        R2.ethg++ <--> Eth10M <--> switch1.ethg++;
        switch3.ethg++ <--> Eth100M <--> hostD2.ethg++;
        switch3.ethg++ <--> Eth100M <--> hostD1.ethg++;
        switch3.ethg++ <--> Eth100M <--> hostD0.ethg++;

        switch2.ethg++ <--> Eth100M <--> R1.ethg++;
        R2.ethg++ <--> Eth100M <--> switch3.ethg++;
}

network IGMP_Simple
{
    @display("bgb=870.638,694.038");

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
        host0: StandardHost {
            @display("p=153.642,245.47401");
        }
        host1: StandardHost {
            @display("p=153.642,425.60602");
        }
        host2: StandardHost {
            @display("p=153.642,611.036");
        }
        R0: Router {
            @display("p=794.7,425.60602");
        }
        switch0: EtherSwitch {
            @display("p=473.28802,422.074");
        }
    connections:
        host2.ethg++ <--> Eth100M <--> switch0.ethg++;
        host1.ethg++ <--> Eth100M <--> switch0.ethg++;
        host0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch0.ethg++ <--> Eth100M <--> R0.ethg++;

}

network IGMP_MultipleRouters
{
    @display("bgb=1019.844,938.904");

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
        host0: StandardHost {
            @display("p=178.068,348.042");
        }
        host1: StandardHost {
            @display("p=178.068,528.808");
        }
        host2: StandardHost {
            @display("p=178.068,714.97");
        }
        switch0: EtherSwitch {
            @display("p=474.848,526.11");
        }
        R0: Router {
            @display("p=733.856,307.572");
        }
        R1: Router {
            @display("p=733.856,528.808");
        }
        R2: Router {
            @display("p=733.856,782.42");
        }
    connections:
        host2.ethg++ <--> Eth100M <--> switch0.ethg++;
        host1.ethg++ <--> Eth100M <--> switch0.ethg++;
        host0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch0.ethg++ <--> Eth100M <--> R1.ethg++;
        switch0.ethg++ <--> Eth100M <--> R0.ethg++;
        switch0.ethg++ <--> Eth100M <--> R2.ethg++;
}

network Multicast_Routing_Simple
{
    @display("bgb=2792.43,1343.604");

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
            @display("p=199.785,567.81");
        }
        hostA1: StandardHost {
            @display("p=199.785,749.1937");
        }
        hostA2: StandardHost {
            @display("p=199.785,935.835");
        }
        hostB0: StandardHost {
            @display("p=1270.7579,1184.422");
        }
        R1: Router {
            @display("p=678.21747,749.1937");
        }
        R2: Router {
            @display("p=1481.202,747.346");
        }
        hostB1: StandardHost {
            @display("p=1481.202,1184.422");
        }
        hostB2: StandardHost {
            @display("p=1710.532,1184.422");
        }
        hostC0: StandardHost {
            @display("p=473.175,1209.225");
        }
        hostC1: StandardHost {
            @display("p=678.21747,1209.225");
        }
        hostC2: StandardHost {
            @display("p=893.77496,1209.225");
        }
        switch0: EtherSwitch {
            @display("p=420.6,746.565");
        }
        switch2: EtherSwitch {
            @display("p=678.21747,993.6675");
        }
        switch1: EtherSwitch {
            @display("p=1478.504,979.37396");
        }
        switch3: EtherSwitch {
            @display("p=1481.202,523.412");
        }
        hostD0: StandardHost {
            @display("p=1246.476,294.082");
        }
        hostD1: StandardHost {
            @display("p=1475.806,296.78");
        }
        hostD2: StandardHost {
            @display("p=1707.834,291.384");
        }
        R3: Router {
            @display("p=2282.508,747.346");
        }
        switch4: EtherSwitch {
            @display("p=2282.508,992.864");
        }
        hostE0: StandardHost {
            @display("p=2072.064,1197.912");
        }
        hostE2: StandardHost {
            @display("p=2514.536,1197.912");
        }
        hostE1: StandardHost {
            @display("p=2285.206,1197.912");
        }
    connections:
        hostA2.ethg++ <--> Eth100M <--> switch0.ethg++;
        hostA1.ethg++ <--> Eth100M <--> switch0.ethg++;
        hostA0.ethg++ <--> Eth100M <--> switch0.ethg++;
        switch0.ethg++ <--> Eth100M <--> R1.ethg++;

        switch1.ethg++ <--> Eth100M <--> hostB0.ethg++;
        switch1.ethg++ <--> Eth100M <--> hostB1.ethg++;
        switch1.ethg++ <--> Eth100M <--> hostB2.ethg++;
        switch2.ethg++ <--> Eth100M <--> hostC0.ethg++;
        switch2.ethg++ <--> Eth100M <--> hostC1.ethg++;
        switch2.ethg++ <--> Eth100M <--> hostC2.ethg++;

        R1.ethg++ <--> Eth100M <--> R2.ethg++;
        R2.ethg++ <--> Eth10M <--> switch1.ethg++;
        switch3.ethg++ <--> Eth100M <--> hostD2.ethg++;
        switch3.ethg++ <--> Eth100M <--> hostD1.ethg++;
        switch3.ethg++ <--> Eth100M <--> hostD0.ethg++;

        switch2.ethg++ <--> Eth100M <--> R1.ethg++;
        R2.ethg++ <--> Eth100M <--> switch3.ethg++;
        switch4.ethg++ <--> Eth100M <--> hostE0.ethg++;
        switch4.ethg++ <--> Eth100M <--> hostE1.ethg++;
        switch4.ethg++ <--> Eth100M <--> hostE2.ethg++;
        R3.ethg++ <--> Eth100M <--> switch4.ethg++;
        R2.ethg++ <--> Eth100M <--> R3.ethg++;
}

network Multicast_Routing
{
    @display("bgb=1191.2709,1276.3616");

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
        R1: Router {
            @display("p=169.974,596.258");
        }
        R2: Router {
            @display("p=482.942,596.258");
        }
        R3: Router {
            @display("p=785.118,596.258");
        }
        R4: Router {
            @display("p=1060.314,596.258");
        }
        R5: Router {
            @display("p=169.974,890.33997");
        }
        R6: Router {
            @display("p=542.298,890.33997");
        }
        source1: StandardHost {
            @display("p=169.974,342.646");
        }
        source2: StandardHost {
            @display("p=1060.314,342.646");
        }
        receiver1: StandardHost {
            @display("p=168.10616,1145.6123");
        }
        receiver2: StandardHost {
            @display("p=542.298,1146.65");
        }
    connections:
        R5.ethg++ <--> Eth100M <--> R6.ethg++;
        R4.ethg++ <--> Eth100M <--> R3.ethg++;
        R3.ethg++ <--> Eth100M <--> R2.ethg++;
        R2.ethg++ <--> Eth100M <--> R1.ethg++;
        R5.ethg++ <--> Eth100M <--> R1.ethg++;
        R3.ethg++ <--> Eth100M <--> R5.ethg++;
        receiver2.ethg++ <--> Eth100M <--> R6.ethg++;
        receiver1.ethg++ <--> Eth100M <--> R5.ethg++;
        R1.ethg++ <--> Eth100M <--> source1.ethg++;
        R4.ethg++ <--> Eth100M <--> source2.ethg++;
}

network PruneOverride
{
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=98.799995,48.1");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=219,48");
        }
        R1: Router {
            @display("p=248,200");
        }
        R2: Router {
            @display("p=486,104");
        }
        R3: Router {
            @display("p=486,293");
        }
        switch: EtherSwitch {
            @display("p=382,199");
        }
        source: StandardHost {
            @display("p=98,200");
        }
        receiver1: StandardHost {
            @display("p=643,104");
        }
        receiver2: StandardHost {
            @display("p=643,293");
        }
    connections allowunconnected:
        R1.ethg++ <--> Eth100M <--> source.ethg++;
        R1.ethg++ <--> Eth100M <--> switch.ethg++;
        switch.ethg++ <--> Eth100M <--> R2.ethg++;
        switch.ethg++ <--> Eth100M <--> R3.ethg++;
        R2.ethg++ <--> Eth100M <--> receiver1.ethg++;
        R3.ethg++ <--> Eth100M <--> receiver2.ethg++;
}

network Multicast_Assert
{
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=98.799995,48.1");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=98.799995,144.3");
        }
        R1: Router {
            @display("p=400,200");
        }
        R2: Router {
            @display("p=550,100");
        }
        R3: Router {
            @display("p=550,300");
        }
        R4: Router {
            @display("p=850,199");
        }
        switch: EtherSwitch {
            @display("p=700,199");
        }
        source: StandardHost {
            @display("p=250,200");
        }
        receiver1: StandardHost {
            @display("p=997.1,104");
        }
        receiver2: StandardHost {
            @display("p=997.1,265.19998");
        }
    connections allowunconnected:
        R1.ethg++ <--> Eth100M <--> R2.ethg++;
        R1.ethg++ <--> Eth100M <--> R3.ethg++;
        R1.ethg++ <--> Eth100M <--> source.ethg++;

        switch.ethg++ <--> Eth100M <--> R2.ethg++;
        switch.ethg++ <--> Eth100M <--> R3.ethg++;
        R4.ethg++ <--> Eth100M <--> switch.ethg++;
        R4.ethg++ <--> Eth100M <--> receiver1.ethg++;
        receiver2.ethg++ <--> Eth100M <--> R4.ethg++;
}

network PIM_SM_Network
{
    @display("bgb=885,295");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=99.71,57.46");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=99,147");
        }
        R1: Router {
            @display("p=398.84,147.03");
        }
        R2: Router {
            @display("p=498.55,236.59999");
        }
        R3: Router {
            @display("p=685,236");
        }
        RP: Router {
            @display("p=599.95,57.46");
        }
        Source: StandardHost {
            @display("i=device/server;p=832,236");
        }
        Receiver1: StandardHost {
            @display("p=347.1,236.59999");
        }
        Receiver2: StandardHost {
            @display("p=248.43,147.03");
        }
    connections allowunconnected:
        R2.ethg++ <--> Eth100M <--> RP.ethg++;
        R2.ethg++ <--> Eth100M <--> R3.ethg++;
        R1.ethg++ <--> Eth100M <--> R2.ethg++;
        Receiver2.ethg++ <--> Eth100M <--> R1.ethg++;
        Receiver1.ethg++ <--> Eth100M <--> R2.ethg++;
        R3.ethg++ <--> Eth100M <--> Source.ethg++;
        R3.ethg++ <--> Eth100M <--> RP.ethg++;
}

network PIM_SM_Network_Complex
{
    @display("bgb=1164,295");
    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=99.71,57.46");
        }
        visualizer: IntegratedCanvasVisualizer {
            @display("p=99,147");
        }
        R1: Router {
            @display("p=398.84,147.03");
        }
        R2: Router {
            @display("p=498.55,236.59999");
        }
        R3: Router {
            @display("p=868.39996,232.7");
        }
        R4: Router {
            @display("p=767,57.2");
        }
        R5: Router {
            @display("p=951.6,57.2");
        }
        RP: Router {
            @display("p=599.95,57.46");
        }
        Source: StandardHost {
            @display("i=device/server;p=1029.6,232.7");
        }
        Receiver1: StandardHost {
            @display("p=347.1,236.59999");
        }
        Receiver2: StandardHost {
            @display("p=248.43,147.03");
        }
        Receiver3: StandardHost {
            @display("p=1104,57");
        }
    connections allowunconnected:
        R2.ethg++ <--> Eth100M <--> RP.ethg++;
        R2.ethg++ <--> Eth100M <--> R3.ethg++;
        R1.ethg++ <--> Eth100M <--> R2.ethg++;
        Receiver2.ethg++ <--> Eth100M <--> R1.ethg++;
        Receiver1.ethg++ <--> Eth100M <--> R2.ethg++;
        R3.ethg++ <--> Eth100M <--> Source.ethg++;
        R4.ethg++ <--> Eth100M <--> RP.ethg++;
        R3.ethg++ <--> Eth100M <--> R5.ethg++;
        R5.ethg++ <--> Eth100M <--> R4.ethg++;
        Receiver3.ethg++ <--> Eth100M <--> R5.ethg++;
}
