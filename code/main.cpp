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

#include <fstream>
#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Demo");

bool OnPacketReceived(ns3::Ptr<ns3::NetDevice> /* dev */,
                      ns3::Ptr<const ns3::Packet> pkt,
                      uint16_t /* mode */,
                      const ns3::Address & /* sender */)
{
    std::cout << "Yooo" << "\n";
    return true;
}

void OnSocketReceive(ns3::Ptr<ns3::Socket> socket)
{
    std::cout << "Yooo" << "\n";
}

void OnConnectedSuccess2(ns3::Ptr<ns3::Socket> socket)
{
    std::cout << "Yooo" << "\n";
}

void OnConnectedFailure2(ns3::Ptr<ns3::Socket> socket)
{
    std::cout << "Yooo" << "\n";
}

bool OnConnectionRequest(ns3::Ptr<ns3::Socket>, const ns3::Address &newConnectionCreate)
{
    std::cout << "Yooo" << "\n";
    return true;
}

void OnConnectionCreatedFail(ns3::Ptr<ns3::Socket>, const ns3::Address &newConnectionCreate)
{
    std::cout << "Yooo" << "\n";
}

void OnConnectionCreated(ns3::Ptr<ns3::Socket>, const ns3::Address &newConnectionCreate)
{
    std::cout << "Yooo" << "\n";
}

int main(int argc, char *argv[])
{
    bool tracing = false;
    uint32_t maxBytes = 0;

    //
    // Allow the user to override any of the defaults at
    // run-time, via command-line arguments
    //
    CommandLine cmd(__FILE__);
    cmd.AddValue("tracing", "Flag to enable/disable tracing", tracing);
    cmd.AddValue("maxBytes", "Total number of bytes for application to send", maxBytes);
    cmd.Parse(argc, argv);

    //
    // Explicitly create the nodes required by the topology (shown above).
    //
    NS_LOG_INFO("Create nodes.");
    NodeContainer nodes;
    nodes.Create(2);

    NS_LOG_INFO("Create channels.");

    //
    // Explicitly create the point-to-point link required by the topology (shown above).
    //

    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", DataRateValue(DataRate(5000000)));
    csma.SetChannelAttribute("Delay", TimeValue(MilliSeconds(2)));
    csma.SetDeviceAttribute("Mtu", UintegerValue(1400));
    NetDeviceContainer devices = csma.Install(nodes);

    //
    // Install the internet stack on the nodes
    //
    InternetStackHelper internet;
    internet.Install(nodes);

    //
    // We've got the "hardware" in place.  Now we need to add IP addresses.
    //
    NS_LOG_INFO("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i = ipv4.Assign(devices);

    NS_LOG_INFO("Create Applications.");

    //
    // Create a BulkSendApplication and install it on node 0
    //
    uint16_t port = 9; // well-known echo port number

    ApplicationSenderHelper source("ns3::TcpSocketFactory", InetSocketAddress(i.GetAddress(1), port));
    // Set the amount of data to send in bytes.  Zero is unlimited.
    

    ApplicationContainer sourceApps = source.Install(nodes.Get(0));
    sourceApps.Start(Seconds(0.0));
    sourceApps.Stop(Seconds(10.0));

    //
    // Create a PacketSinkApplication and install it on node 1
    //
    PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port));
    ApplicationContainer sinkApps = sink.Install(nodes.Get(1));
    sinkApps.Start(Seconds(0.0));
    sinkApps.Stop(Seconds(10.0));

    //
    // Now, do the actual simulation.
    //
    NS_LOG_INFO("Run Simulation.");
    Simulator::Stop(Seconds(10.0));
    Simulator::Run();
    Simulator::Destroy();
    NS_LOG_INFO("Done.");

    Ptr<PacketSink> sink1 = DynamicCast<PacketSink>(sinkApps.Get(0));
    std::cout << "Total Bytes Received: " << sink1->GetTotalRx() << std::endl;

    return 0;
}

