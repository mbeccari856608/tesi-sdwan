#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "DeviceApplication.h"
#include <fstream>
#include <string>
#include "ApplicationSenderHelper.h"
#include "ReceiverApplicationHelper.h"
#include "ReceiverApplication.h"
#include "Utils.h"

#include <fstream>
#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Demo");

int main(int argc, char *argv[])
{
    bool tracing = false;
    uint32_t maxBytes = 0;


    ns3::Ptr<Node> cpe = CreateObject<Node>();
    ns3::Ptr<Node> sinkNode = CreateObject<Node>();

    NodeContainer nodes;
    nodes.Add(cpe);
    nodes.Add(sinkNode);



    CsmaHelper slowInterfaceHelper;
    ns3::DataRateValue slowSpeed = Utils::ConvertPacketsPerSecondToBitPerSecond(10);
    slowInterfaceHelper.SetChannelAttribute("DataRate", slowSpeed);
    slowInterfaceHelper.SetChannelAttribute("Delay", TimeValue(MilliSeconds(200)));
    NetDeviceContainer devices = slowInterfaceHelper.Install(nodes);


    InternetStackHelper internet;
    internet.Install(nodes);


    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i = ipv4.Assign(devices);



    uint16_t port = 9; // well-known echo port number
    
    std::vector<ns3::Address> destinations;
    destinations.push_back(InetSocketAddress(i.GetAddress(1), port));


    ApplicationSenderHelper source(destinations, 1280);

    ApplicationContainer sourceApps = source.Install(nodes.Get(0));
    sourceApps.Start(Seconds(0.0));

    //
    // Create a PacketSinkApplication and install it on node 1
    //
    ReceiverApplicationHelper sink(destinations.at(0));
    ApplicationContainer sinkApps = sink.Install(nodes.Get(1));
    sinkApps.Start(Seconds(0.0));

    //
    // Now, do the actual simulation.
    //
    NS_LOG_INFO("Run Simulation.");
    Simulator::Run();
    auto finalTime = Simulator::Now().GetMinutes();
    Simulator::Destroy();

    std::cout << "Finito " << Simulator::IsFinished() << std::endl;
    

    std::cout << "Durata totale " << finalTime << std::endl;

    Ptr<ReceiverApplication> sink1 = DynamicCast<ReceiverApplication>(sinkApps.Get(0));
    std::cout << "Total Bytes Received: " << sink1->GetTotalRx() << std::endl;

    return 0;
}
