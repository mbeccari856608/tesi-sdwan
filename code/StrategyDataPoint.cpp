#include "StrategyDataPoint.h"

StrategyDataPoint::StrategyDataPoint(
    uint32_t totalCost,
    uint32_t currentCost,
    uint32_t totalAmountOfPackets,
    uint32_t currentAmountOfPackets) : totalCost(totalCost), currentCost(currentCost), totalAmountOfPackets(totalAmountOfPackets), currentAmountOfPackets(currentAmountOfPackets)
{
}