#include <memory>
#include <vector>
#include <algorithm>
#include "Strategy.h"
#include "Utils.h"
#include "RandomStrategy.h"
#include "SDWanApplication.h"
#include "SDWanStaticApplication.h"
#include "ISPInterface.h"
#include <algorithm>
#include <random>

RandomStrategy::RandomStrategy(
    std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> availableInterfaces)
    : Strategy(applications, availableInterfaces), rng(std::default_random_engine())
{
    this->rng.seed(Utils::SeedForRandomGeneration);
}

void RandomStrategy::Compute()
{
    // We want to pick applications in a random order, so we create
    // an array with random indexes and use it to decide which applications
    // to consider.
    uint32_t size = applications->size();
    std::vector<uint32_t> indexes(size);
    for (int i = 0; i <= size; ++i)
    {
        indexes[i] = i;
    }

    // We then randomly shuffle the values.
    std::shuffle(std::begin(indexes), std::end(indexes), rng);

    uint32_t currentCost = 0;
    uint32_t currentPendingPackages = 0;

    for (size_t i = 0; i < applications->size(); i++)
    {
        auto applicationIndex = indexes.at(i);
        auto currentApplication = applications->at(i);
        ;
        if (currentApplication->pendingpackets.empty())
        {
            continue;
        }

        ns3::Time currentTime = ns3::Simulator::Now();
        uint32_t applicationId = currentApplication->applicationId;

        std::srand(std::time(nullptr));
        while (!currentApplication->pendingpackets.empty())
        {
            currentApplication->pendingpackets.pop();
            currentPendingPackages++;
            SendPacketInfo packetInfo;

            packetInfo.dateEnqueued = currentTime;
            packetInfo.originatedFrom = applicationId;

            uint32_t amountOfInterfaces = this->availableInterfaces->size();
            uint32_t randomIndex = this->rng() % this->availableInterfaces->size();
            this->availableInterfaces->at(randomIndex)->enqueuePacket(packetInfo);
            currentCost += this->availableInterfaces->at(randomIndex)->cost;
        }
    }

    uint32_t totalCost = strategyData.size() == 0 ? currentCost : strategyData.back().totalCost + currentCost;
    uint32_t totalAmountOfPackets = strategyData.size() == 0 ? currentPendingPackages : strategyData.back().totalAmountOfPackets + currentPendingPackages;
    StrategyDataPoint dataPoint = StrategyDataPoint(totalCost, currentCost, totalAmountOfPackets, currentPendingPackages);
    this->strategyData.push_back(dataPoint);

    if (this->getAllDataHasBeenSent())
    {
        Utils::printResultsToFile("outputRandom.csv", this->strategyData);
    }
}