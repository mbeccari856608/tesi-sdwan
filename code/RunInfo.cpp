#include "RunInfo.h"
#include "RunParameters.h"
#include <cstdint>
#include "StrategyTypes.h"
#include <vector>

RunInfo::RunInfo(uint32_t totalPackets, uint32_t totalCost,
                 std::vector<double> applicationErrorRates, std::vector<double> applicationBandWidth, RunParameters parameters)
    : totalAmountOfPackets(totalPackets),
      totalCost(totalCost),
      applicationErrorRates(applicationErrorRates),
      applicationBandWidth(applicationBandWidth),
      parameters(parameters) {}

double RunInfo::getAveragePacketCost()
{
    if (totalAmountOfPackets == 0)
    {
        return 0;
    }
    return (double)totalAmountOfPackets / totalCost;
}