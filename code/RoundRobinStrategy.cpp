#include "RoundRobinStrategy.h"
#include "Utils.h"
RoundRobinStrategy::RoundRobinStrategy(
    std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> availableInterfaces)
    : Strategy(applications, availableInterfaces)
{
}

void RoundRobinStrategy::Compute()
{

    uint32_t currentIndex = 0;
    uint32_t totalInterfacesAmount = availableInterfaces->size();

    uint32_t currentCost = 0;
    uint32_t currentPendingPackages = 0;
    for (size_t i = 0; i < applications->size(); i++)
    {
        auto currentApplication = applications->at(i);

        if (currentApplication->pendingpackets.empty())
        {
            continue;
        }

        ns3::Time currentTime = ns3::Simulator::Now();
        uint32_t applicationId = currentApplication->applicationId;

        while (!currentApplication->pendingpackets.empty())
        {
            currentPendingPackages++;
            currentApplication->pendingpackets.pop();
            SendPacketInfo packetInfo;

            packetInfo.dateEnqueued = currentTime;
            packetInfo.originatedFrom = applicationId;

            this->availableInterfaces->at(currentIndex)->enqueuePacket(packetInfo);
            currentCost += this->availableInterfaces->at(currentIndex)->cost;
            currentIndex = (currentIndex + 1) % totalInterfacesAmount;
        }
    }
    uint32_t totalCost = strategyData.size() == 0 ? currentCost : strategyData.back().totalCost + currentCost;
    uint32_t totalAmountOfPackets = strategyData.size() == 0 ? currentPendingPackages : strategyData.back().totalAmountOfPackets + currentPendingPackages;
    StrategyDataPoint dataPoint = StrategyDataPoint(totalCost, currentCost, totalAmountOfPackets, currentPendingPackages);
    this->strategyData.push_back(dataPoint);

    if (this->getAllDataHasBeenSent())
    {
        Utils::printResultsToFile("outputRoundRobin.csv", this->strategyData);
    }
}