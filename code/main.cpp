#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include <fstream>
#include <string>
#include "ApplicationSenderHelper.h"
#include "ReceiverApplicationHelper.h"
#include "ReceiverApplication.h"
#include "Utils.h"
#include "SDWanStaticApplication.h"
#include "SenderApplication.h"
#include <boost/range/algorithm/sort.hpp>
#include <ranges>
#include <unordered_map>
#include "SinApplication.h"
#include <fstream>
#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Demo");

uint32_t initialApplicationId = 0;

uint32_t getNextApplicationId()
{
    initialApplicationId++;
    return initialApplicationId;
}

int main(int argc, char *argv[])
{
    auto timeSinceEpoch = std::chrono::high_resolution_clock::now().time_since_epoch();

    RngSeedManager::SetSeed(timeSinceEpoch.count());

    bool tracing = false;
    uint32_t maxBytes = 0;

    ns3::Ptr<Node> cpe = CreateObject<Node>();
    ns3::Ptr<Node> sinkNode = CreateObject<Node>();

    NodeContainer nodes;
    nodes.Add(cpe);
    nodes.Add(sinkNode);

    std::vector<uint32_t> costs;
    std::string errorModelType = "ns3::RateErrorModel";
    ObjectFactory factory;
    factory.SetTypeId(errorModelType);

    CsmaHelper slowInterfaceHelper;
    ns3::DataRateValue slowSpeed = Utils::ConvertPacketsPerSecondToBitPerSecond(10);
    slowInterfaceHelper.SetChannelAttribute("DataRate", slowSpeed);
    slowInterfaceHelper.SetChannelAttribute("Delay", TimeValue(MilliSeconds(300)));
    NetDeviceContainer slowDevices = slowInterfaceHelper.Install(nodes);
    costs.push_back(10);

    Ptr<RateErrorModel> em = factory.Create<RateErrorModel>();
    em->SetRate(0.2);
    em->SetAttribute("ErrorUnit", StringValue("ERROR_UNIT_PACKET"));
    slowDevices.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(em));

    CsmaHelper mediumInterfaceHelper;
    ns3::DataRateValue mediumSpeed = Utils::ConvertPacketsPerSecondToBitPerSecond(20);
    mediumInterfaceHelper.SetChannelAttribute("DataRate", mediumSpeed);
    mediumInterfaceHelper.SetChannelAttribute("Delay", TimeValue(MilliSeconds(50)));
    NetDeviceContainer mediumDevices = mediumInterfaceHelper.Install(nodes);
    costs.push_back(30);

    Ptr<RateErrorModel> em2 = factory.Create<RateErrorModel>();
    em2->SetRate(0.1);
    em2->SetAttribute("ErrorUnit", StringValue("ERROR_UNIT_PACKET"));
    mediumDevices.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(em2));

    CsmaHelper fastInterfaceHelper;
    ns3::DataRateValue fastSpeed = Utils::ConvertPacketsPerSecondToBitPerSecond(30);
    fastInterfaceHelper.SetChannelAttribute("DataRate", fastSpeed);
    fastInterfaceHelper.SetChannelAttribute("Delay", TimeValue(MilliSeconds(30)));
    NetDeviceContainer fastDevices = fastInterfaceHelper.Install(nodes);
    costs.push_back(100);

    Ptr<RateErrorModel> em3 = factory.Create<RateErrorModel>();
    em3->SetRate(0.01);
    em3->SetAttribute("ErrorUnit", StringValue("ERROR_UNIT_PACKET"));
    fastDevices.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(em3));

    InternetStackHelper internet;

    internet.Install(nodes);

    Ipv4AddressHelper ipv4First;
    ipv4First.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer firstInterfaceContainer = ipv4First.Assign(slowDevices);

    Ipv4AddressHelper ipv4Second;
    ipv4Second.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer secondInterfaceContainer = ipv4Second.Assign(mediumDevices);

    Ipv4AddressHelper ipv4Third;
    ipv4Third.SetBase("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer thirdInterfaceContainer = ipv4Third.Assign(fastDevices);

    std::vector<ns3::Address> destinations;
    destinations.push_back(InetSocketAddress(firstInterfaceContainer.GetAddress(1), Utils::ConnectionPort));
    destinations.push_back(InetSocketAddress(secondInterfaceContainer.GetAddress(1), Utils::ConnectionPort));
    destinations.push_back(InetSocketAddress(thirdInterfaceContainer.GetAddress(1), Utils::ConnectionPort));


    ns3::DataRateValue slowSpeedRequirement = Utils::ConvertPacketsPerSecondToBitPerSecond(1);

    std::vector<std::shared_ptr<SDWanApplication>> applications;
    std::unique_ptr<ApplicationSenderHelper> source;

    // std::shared_ptr<SDWanApplication> testApplication = std::make_shared<SDWanStaticApplication>(getNextApplicationId(), slowSpeedRequirement.Get(), 200, 10, 100);

    std::shared_ptr<SDWanApplication> firstSinApplication = std::make_shared<SinApplication>(getNextApplicationId(), 200, 10, 12);
    std::shared_ptr<SDWanApplication> secondSinApplication = std::make_shared<SinApplication>(getNextApplicationId(), 200, 10, 8);
    std::shared_ptr<SDWanApplication> thirdSinApplication = std::make_shared<SinApplication>(getNextApplicationId(), 150, 20, 4);

    applications.push_back(std::move(firstSinApplication));
    applications.push_back(std::move(secondSinApplication));
    applications.push_back(std::move(thirdSinApplication));

    source = std::make_unique<ApplicationSenderHelper>(destinations, applications, costs, RANDOM);

    ApplicationContainer sourceApps = source->Install(nodes.Get(0));
    sourceApps.Start(Seconds(0.0));

    Ptr<SenderApplication> senderApplication = DynamicCast<SenderApplication>(sourceApps.Get(0));

    //
    // Create a PacketSinkApplication and install it on node 1
    //
    ReceiverApplicationHelper sink(senderApplication->getInterfaces());
    ApplicationContainer sinkApps = sink.Install(nodes.Get(1));
    sinkApps.Start(Seconds(0.0));

    //
    // Now, do the actual simulation.
    //
    NS_LOG_INFO("Run Simulation.");
    Simulator::Run();
    auto finalTime = Simulator::Now().GetMinutes();
    Simulator::Stop();

    std::cout << "Finito " << Simulator::IsFinished() << std::endl;

    std::cout << "Durata totale " << finalTime << std::endl;

    Ptr<ReceiverApplication> sink1 = DynamicCast<ReceiverApplication>(sinkApps.Get(0));
    std::cout << "Total Bytes Received: " << sink1->GetTotalRx() << std::endl;

    auto allPackets = sink1->getReceivedPacketInfo();

    std::cout << "Totale pacchetti ricevuti: " << std::to_string(allPackets.size()) << std::endl;

    std::ranges::sort(allPackets, {}, &ReceivedPacketInfo::fromApplication);

    std::map<uint32_t, std::vector<ReceivedPacketInfo>> flowGroups;


    for (const auto &pInfo : allPackets)
    {
        flowGroups[pInfo.fromApplication].push_back(pInfo);
    }

    for (const auto &groupOfPackets : flowGroups)
    {
        uint32_t applicationId = groupOfPackets.first;

        if (groupOfPackets.second.size() < 1){
            std::cout << "Nessun paccketto inviato per l'applicazione " << applicationId << "\n";
            continue;
        }

        std::cout << std::string(20, '-') << std::endl;
        std::cout << "Informazione per l'applicazione " << std::to_string(applicationId) << " " << "\n";

        auto maybeApplication = std::find_if(applications.begin(), applications.end(), [applicationId](const std::shared_ptr<SDWanApplication> item)
                                             { return item->applicationId == applicationId; });

        if (maybeApplication == applications.end())
        {
            std::cout << "Applicazione non trovata" << std::endl;
            continue;
        }

        std::shared_ptr<SDWanApplication> currentApplication = *maybeApplication;

        // Ritardo Medio:
        double totalDelay = 0.0;
        for (size_t i = 0; i < groupOfPackets.second.size(); i++)
        {
            totalDelay += (double)groupOfPackets.second.at(i).getDelayInMilliSeconds();
        }

        double averageDelay = totalDelay / groupOfPackets.second.size();
        std::cout << "Average delay: " << averageDelay << "ms" << std::endl;

        double totalBandwidth = 0.0;
        for (size_t i = 0; i < groupOfPackets.second.size(); i++)
        {
            totalBandwidth += (double)groupOfPackets.second.at(i).getDataRateInPacketPerSeconds();
        }

        double averageBandwidth = totalBandwidth / groupOfPackets.second.size();
        std::cout << "Average bandwidth: " << averageBandwidth << " packets/s " << std::endl;




        double generatedPackages = (double)currentApplication->generatedPackets;
        double receivedPackages = groupOfPackets.second.size();


        
        double successRate = (receivedPackages / generatedPackages) * 100;
        double errorRate = 100 - successRate;
        
        std::cout << "Error rate: " << std::to_string(errorRate) << "% " << std::endl;
        
        
        double totalCost = 0.0;
        for (size_t i = 0; i < groupOfPackets.second.size(); i++)
        {
            totalCost += (double)groupOfPackets.second.at(i).getCost();
        }
        
        std::cout << "Costo della trasmissione: " << totalCost << std::endl;
        
        std::map<std::shared_ptr<ISPInterface>, std::vector<ReceivedPacketInfo>> packets;
        for (size_t i = 0; i < groupOfPackets.second.size(); i++)
        {
            packets[groupOfPackets.second.at(i).fromInterface].push_back(groupOfPackets.second.at(i));
        }
        
        std::cout << "Totale pacchetti ricevuti: " << receivedPackages << std::endl;
        for (const auto &fromInterfaceInfo : packets)
        {
            std::cout << "\t Pacchetti dall'interfaccia " << fromInterfaceInfo.first->interfaceId << ": " << fromInterfaceInfo.second.size() << std::endl;

        }

    }

    Simulator::Destroy();

    return 0;
}
