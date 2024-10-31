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

#include <fstream>
#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Demo");

bool OnPacketReceived(ns3::Ptr<ns3::NetDevice> /* dev */,
                  ns3::Ptr<const ns3::Packet> pkt,
                  uint16_t /* mode */,
                  const ns3::Address& /* sender */)
{
    std::cout << "Yooo"  << "\n";
    return true;
}


int main(int argc, char *argv[])
{
    uint16_t port = 8080; // well-known echo port number


    DeviceApplication application;
    InternetStackHelper internet;

    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");

    // Creo il nodo
    NodeContainer sinkNode;
    sinkNode.Create(1);
    // Installo internet
    internet.Install(sinkNode);
    // Device e canale
    ns3::Ptr<CsmaNetDevice> sinkDevice = CreateObject<CsmaNetDevice>();

    sinkDevice->SetReceiveCallback(ns3::MakeCallback(&OnPacketReceived));
    sinkDevice->SetReceiveEnable(true);
    ns3::Ptr<DropTailQueue<ns3::Packet>> sinkQueue = CreateObject<DropTailQueue<ns3::Packet>>();
    sinkDevice->SetQueue(sinkQueue);
    ns3::Ptr<CsmaChannel> sinkChannel = CreateObject<CsmaChannel>();

    sinkDevice->Attach(sinkChannel);
    sinkNode.Get(0)->AddDevice(sinkDevice);

    // Associa l'ip
    NetDeviceContainer sinkNetDeviceContainer(sinkDevice);
    Ipv4InterfaceContainer sinkInterfaceContainer = ipv4.Assign(sinkNetDeviceContainer);
    std::cout << "Indirizzo ip: " << sinkInterfaceContainer.GetAddress(0) << "\n";

    // Aggiunge l'app
    PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(sinkInterfaceContainer.GetAddress(0) , port));
    ApplicationContainer sinkApps = sink.Install(sinkNode.Get(0));
    
    sinkApps.Start(Seconds(0.0));
    sinkApps.Stop(Seconds(10.0));



    // Creo il nodo
    NodeContainer cpe;
    cpe.Create(1);
    // Installo internet
    internet.Install(cpe);
    // Device e canale
    ns3::Ptr<CsmaNetDevice> cpeDevice = CreateObject<CsmaNetDevice>();
    ns3::Ptr<DropTailQueue<ns3::Packet>> cpeQueue = CreateObject<DropTailQueue<ns3::Packet>>();
    cpeDevice->SetQueue(cpeQueue);
    ns3::Ptr<CsmaChannel> channel = CreateObject<CsmaChannel>();

    cpeDevice->Attach(channel);
    cpe.Get(0)->AddDevice(cpeDevice);

    // Associa l'ip
    NetDeviceContainer csmaNetDeviceContainer(cpeDevice);
    Ipv4InterfaceContainer cpeInterfaceContainer = ipv4.Assign(csmaNetDeviceContainer);
    std::cout << "Indirizzo ip: " << cpeInterfaceContainer.GetAddress(0) << "\n";

    
    // Aggiunge l'app
    ns3::Ptr<DeviceApplication> app = CreateObject<DeviceApplication>();
    ns3::Ptr<Socket> sourceSocket = Socket::CreateSocket(cpe.Get(0), TcpSocketFactory::GetTypeId());
    cpe.Get(0)->AddApplication(app);


    app->Setup(sourceSocket,  InetSocketAddress(sinkInterfaceContainer.GetAddress(0), port));
    app->SetStartTime(Seconds(0.));
    app->SetStopTime(Seconds(10.));

    //
    // Now, do the actual simulation.
    //
    NS_LOG_INFO("Run Simulation.");
    Simulator::Stop(Seconds(20.0));
    Simulator::Run();
    Simulator::Destroy();
    NS_LOG_INFO("Done.");



     Ptr<PacketSink> sink1 = DynamicCast<PacketSink>(sinkApps.Get(0));
     if (sink1 == nullptr)
     {
         std::cout << "Sink non valido" << std::endl;
     }
     else
     {

         std::cout << "Total Bytes Received: " << sink1->GetTotalRx() << std::endl;
     }

    return 0;
}
