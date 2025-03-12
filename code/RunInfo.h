#ifndef RUN_INFO_H
#define RUN_INFO_H

#include <cstdint>
#include "RunParameters.h"
#include "StrategyTypes.h"
#include <vector>

/**
 * This class is used to represent the set of results after a simulation is ran.
 *
 */
class RunInfo
{
public:
    RunInfo(uint32_t totalPackets, uint32_t totalCost,
            std::vector<double> applicationErrorRates, std::vector<double> applicationBandWidth, RunParameters parameters);

    uint32_t totalAmountOfPackets;

    uint32_t totalCost;

    std::vector<double> applicationErrorRates;

    std::vector<double> applicationBandWidth;


    RunParameters parameters;

    double getAveragePacketCost();
};

#endif // RUN_INFO_H