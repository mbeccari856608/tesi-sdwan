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
#include "RunParameters.h"
#include "RunInfo.h"
#include <random>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Demo");

RunInfo RunSimulation(const RunParameters &runParameters);

int main(int argc, char *argv[])
{
    StrategyTypes strategies[] = {
        ROUND_ROBIN,
        RANDOM,
        LINEAR,
        REACTIVE,
    };

    std::default_random_engine gen;
    gen.seed(Utils::SeedForRandomGeneration);
    for (StrategyTypes strategy : strategies)
    {
        std::vector<RunInfo> results;
        uint32_t innerMost = 0;
        auto noiseCombinations = Utils::getPermutationsWithRepetitionOfKelementsOfLengthN(3, 3);
        auto peakCombinations = Utils::getPermutationsWithRepetitionOfKelementsOfLengthN(3, 12);
        auto shiftCombinations = Utils::getPermutationsWithRepetitionOfKelementsOfLengthN(3, 3);
        auto totalCount = noiseCombinations->size() * peakCombinations->size() * shiftCombinations->size();

        for (uint32_t currentPeakCombination = 0; currentPeakCombination < peakCombinations->size(); currentPeakCombination++)
        {
            for (uint32_t currentNoiseCombination = 0; currentNoiseCombination < noiseCombinations->size(); currentNoiseCombination++)
            {
                for (uint32_t currentShiftCombination = 0; currentShiftCombination < shiftCombinations->size(); currentShiftCombination++)
                {
                    std::uniform_int_distribution<int> dist(1, 100000);
                    innerMost++;
                    if (dist(gen) == 1)
                    {
                        std::cout << "Progresso corrente: " << ((double) innerMost / totalCount) * 100 << "%" << std::endl;
                        auto currentPeaks =  peakCombinations->at(currentPeakCombination);
                        auto currentNoises = noiseCombinations->at(currentNoiseCombination);
                        auto currentShifts = shiftCombinations->at(currentShiftCombination);

                        RunParameters parameters(
                            currentPeaks.at(0),
                            currentPeaks.at(1),
                            currentPeaks.at(2),
                            currentNoises.at(0),
                            currentNoises.at(1),
                            currentNoises.at(2),
                            currentShifts.at(0) * 50,
                            currentShifts.at(1) * 50,
                            currentShifts.at(2) * 50,
                            strategy);

                        RunInfo result = RunSimulation(parameters);
                        results.push_back(result);
                    }
                }
            }
        }
    
        std::string strategyName = "runs_data_" + std::to_string(strategy) + ".csv"; 
    
        Utils::printResultsToFile(strategyName, results);
    }

    return 0;
}

RunInfo RunSimulation(const RunParameters &runParameters)
{

    uint32_t initialApplicationId = 0;
    auto timeSinceEpoch = std::chrono::high_resolution_clock::now().time_since_epoch();

    RngSeedManager::SetSeed(Utils::SeedForRandomGeneration);

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
    slowInterfaceHelper.SetChannelAttribute("Delay", TimeValue(MilliSeconds(150)));
    NetDeviceContainer slowDevices = slowInterfaceHelper.Install(nodes);
    costs.push_back(10);

    Ptr<RateErrorModel> em = factory.Create<RateErrorModel>();
    em->SetRate(0.2);
    em->SetAttribute("ErrorUnit", StringValue("ERROR_UNIT_PACKET"));
    slowDevices.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(em));

    CsmaHelper mediumInterfaceHelper;
    ns3::DataRateValue mediumSpeed = Utils::ConvertPacketsPerSecondToBitPerSecond(20);
    mediumInterfaceHelper.SetChannelAttribute("DataRate", mediumSpeed);
    mediumInterfaceHelper.SetChannelAttribute("Delay", TimeValue(MilliSeconds(100)));
    NetDeviceContainer mediumDevices = mediumInterfaceHelper.Install(nodes);
    costs.push_back(20);

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

    uint32_t firstApplicationRequiredDelay = 200;
    uint32_t firstApplicationRequiredErrorRate = 20;

    uint32_t secondApplicationRequiredDelay = 150;
    uint32_t secondApplicationRequiredErrorRate = 10;

    uint32_t thirdApplicationRequiredDelay = 100;
    uint32_t thirdApplicationRequiredErrorRate = 5;

    // 200 150 350
    // 3 2 1
    std::shared_ptr<SinApplication> firstSinApplication = std::make_shared<SinApplication>(
        1,
        firstApplicationRequiredDelay,
        firstApplicationRequiredErrorRate,
        runParameters.applicationOnePeak,
        runParameters.applicationOneNoise,
        runParameters.applicationOneShift);

    std::shared_ptr<SinApplication> secondSinApplication = std::make_shared<SinApplication>(
        2,
        secondApplicationRequiredDelay,
        secondApplicationRequiredErrorRate,
        runParameters.applicationTwoPeak,
        runParameters.applicationTwoNoise,
        runParameters.applicationTwoShift);

    std::shared_ptr<SinApplication> thirdSinApplication = std::make_shared<SinApplication>(
        3,
        thirdApplicationRequiredDelay,
        thirdApplicationRequiredErrorRate,
        runParameters.applicationThreePeak,
        runParameters.applicationThreeNoise,
        runParameters.applicationThreeShift);

    if (runParameters.strategyType != LINEAR)
    {

        applications.push_back(std::move(firstSinApplication));
        applications.push_back(std::move(secondSinApplication));
        applications.push_back(std::move(thirdSinApplication));
    }
    else
    {
        auto firstApplicationRequiredDataRate = firstSinApplication->getRequiredDataRate();
        auto firstApplicationTotalAmountOfPackages = firstSinApplication->getTotalData();
        std::shared_ptr<SDWanStaticApplication> firstStaticApplication = std::make_shared<SDWanStaticApplication>(1, firstApplicationRequiredDataRate, firstApplicationRequiredDelay, firstApplicationRequiredErrorRate, firstApplicationTotalAmountOfPackages);

        auto secondApplicationRequiredDataRate = secondSinApplication->getRequiredDataRate();
        auto secondApplicationTotalAmountOfPackages = secondSinApplication->getTotalData();
        std::shared_ptr<SDWanStaticApplication> secondStaticApplication = std::make_shared<SDWanStaticApplication>(2, secondApplicationRequiredDataRate, secondApplicationRequiredDelay, secondApplicationRequiredErrorRate, secondApplicationTotalAmountOfPackages);

        auto thirdApplicationRequiredDataRate = thirdSinApplication->getRequiredDataRate();
        auto thirdApplicationTotalAmountOfPackages = thirdSinApplication->getTotalData();
        std::shared_ptr<SDWanStaticApplication> thirdStaticApplication = std::make_shared<SDWanStaticApplication>(3, thirdApplicationRequiredDataRate, thirdApplicationRequiredDelay, thirdApplicationRequiredErrorRate, thirdApplicationTotalAmountOfPackages);

        applications.push_back(std::move(firstStaticApplication));
        applications.push_back(std::move(secondStaticApplication));
        applications.push_back(std::move(thirdStaticApplication));
    }

    source = std::make_unique<ApplicationSenderHelper>(destinations, applications, costs, runParameters.strategyType);

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



    Ptr<ReceiverApplication> sink1 = DynamicCast<ReceiverApplication>(sinkApps.Get(0));

    auto allPackets = sink1->getReceivedPacketInfo();


    std::ranges::sort(allPackets, {}, &ReceivedPacketInfo::fromApplication);

    std::map<uint32_t, std::vector<ReceivedPacketInfo>> flowGroups;

    for (const auto &pInfo : allPackets)
    {
        flowGroups[pInfo.fromApplication].push_back(pInfo);
    }

    uint32_t totalCost = 0;

    std::vector<double> errorRates;
    std::vector<double> bandWidth;

    for (const auto &groupOfPackets : flowGroups)
    {
        uint32_t applicationId = groupOfPackets.first;

        if (groupOfPackets.second.size() < 1)
        {
            std::cout << "Nessun paccketto inviato per l'applicazione " << applicationId << "\n";
            continue;
        }

        auto maybeApplication = std::find_if(applications.begin(), applications.end(), [applicationId](const std::shared_ptr<SDWanApplication> item)
                                             { return item->applicationId == applicationId; });

        if (maybeApplication == applications.end())
        {
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

        double totalBandwidth = 0.0;
        for (size_t i = 0; i < groupOfPackets.second.size(); i++)
        {
            totalBandwidth += (double)groupOfPackets.second.at(i).getDataRateInPacketPerSeconds();
        }

        double averageBandwidth = totalBandwidth / groupOfPackets.second.size();
        bandWidth.push_back(averageBandwidth);

        double generatedPackages = (double)currentApplication->generatedPackets;
        double receivedPackages = groupOfPackets.second.size();

        double successRate = (receivedPackages / generatedPackages) * 100;
        double errorRate = 100 - successRate;
        errorRates.push_back(errorRate);

        double totalCostForInterface = 0.0;
        for (size_t i = 0; i < groupOfPackets.second.size(); i++)
        {
            totalCostForInterface += (double)groupOfPackets.second.at(i).getCost();
            totalCost += (double)groupOfPackets.second.at(i).getCost();
        }
        std::map<std::shared_ptr<ISPInterface>, std::vector<ReceivedPacketInfo>> packets;
        for (size_t i = 0; i < groupOfPackets.second.size(); i++)
        {
            packets[groupOfPackets.second.at(i).fromInterface].push_back(groupOfPackets.second.at(i));
        }
    }

    //

    Simulator::Destroy();

    return RunInfo(allPackets.size(), totalCost, errorRates, bandWidth, runParameters);
}