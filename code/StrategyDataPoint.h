#ifndef STRATEGY_DATA_POINT_H
#define STRATEGY_DATA_POINT_H
#include <cstdint>

/**
 * This class is used to group data about various metrics when executing the reactive strategy.
 * 
 */
class StrategyDataPoint{

public:

    StrategyDataPoint(
        uint32_t totalCost,
        uint32_t currentCost,
        uint32_t totalAmountOfPackets,
        uint32_t currentAmountOfPackets);

    uint32_t totalCost;

    uint32_t currentCost;

    uint32_t totalAmountOfPackets;

    uint32_t currentAmountOfPackets;

};

#endif /* SIN_GENERATOR_H */
