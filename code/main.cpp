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
    uint16_t port = 8080; // well-known echo port number

    DeviceApplication application;
    InternetStackHelper internet;
    NetDeviceContainer devices;
    ns3::Ptr<CsmaChannel> channel = CreateObject<CsmaChannel>();
    Ipv4StaticRoutingHelper ipv4RoutingHelper;

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
    std::cout << "Abilitato alla ricezione " << sinkDevice->IsReceiveEnabled() << std::endl;

    ns3::Ptr<DropTailQueue<ns3::Packet>> sinkQueue = CreateObject<DropTailQueue<ns3::Packet>>();
    sinkDevice->SetQueue(sinkQueue);

    sinkDevice->Attach(channel);
    sinkNode.Get(0)->AddDevice(sinkDevice);

    // Associa l'ip
    NetDeviceContainer sinkNetDeviceContainer(sinkDevice);
    Ipv4InterfaceContainer sinkInterfaceContainer = ipv4.Assign(sinkNetDeviceContainer);
    std::cout << "Indirizzo ip: " << sinkInterfaceContainer.GetAddress(0) << "\n";

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Aggiunge l'app
    ns3::Address sinkAddress = InetSocketAddress(sinkInterfaceContainer.GetAddress(0), port);
    PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory", sinkAddress);
    ApplicationContainer sinkApps = packetSinkHelper.Install(sinkNode.Get(0));


    sinkApps.Start(Seconds(0.));
    sinkApps.Stop(Seconds(20.));

    // Creo il nodo
    NodeContainer cpe;
    cpe.Create(1);
    // Installo internet
    internet.Install(cpe);
    // Device e canale
    ns3::Ptr<CsmaNetDevice> cpeDevice = CreateObject<CsmaNetDevice>();
    cpeDevice->SetSendEnable(true);
    std::cout << "Abilitato all'invio " << cpeDevice->IsSendEnabled() << std::endl;

    ns3::Ptr<DropTailQueue<ns3::Packet>> cpeQueue = CreateObject<DropTailQueue<ns3::Packet>>();
    cpeDevice->SetQueue(cpeQueue);

    cpeDevice->Attach(channel);
    cpe.Get(0)->AddDevice(cpeDevice);

    // Associa l'ip
    NetDeviceContainer csmaNetDeviceContainer(cpeDevice);
    Ipv4InterfaceContainer cpeInterfaceContainer = ipv4.Assign(csmaNetDeviceContainer);
    std::cout << "Indirizzo ip: " << cpeInterfaceContainer.GetAddress(0) << "\n";

    // Aggiunge l'app
    ns3::Ptr<DeviceApplication> app = CreateObject<DeviceApplication>();
    ns3::Ptr<Socket> sourceSocket = Socket::CreateSocket(cpe.Get(0), TcpSocketFactory::GetTypeId());
    sourceSocket->Bind(InetSocketAddress(cpeInterfaceContainer.GetAddress(0), port));
    cpe.Get(0)->AddApplication(app);

    Ptr<Ipv4StaticRouting> staticRoutingB = ipv4RoutingHelper.GetStaticRouting(cpeInterfaceContainer.Get(0).first);
    ns3::Ipv4Address sourceAddress = cpeInterfaceContainer.GetAddress(0);
    ns3::Ipv4Address destinationAddress = sinkInterfaceContainer.GetAddress(0);

    staticRoutingB->AddHostRouteTo(destinationAddress, 0);

    app->Setup(sourceSocket, sinkAddress);
    app->SetStartTime(Seconds(1.));
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
