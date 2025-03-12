#ifndef RUN_PARAMETERS_H
#define RUN_PARAMETERS_H

#include <cstdint>
#include "StrategyTypes.h"
class RunParameters
{

public:
    RunParameters(uint32_t appOnePeak, uint32_t appTwoPeak, uint32_t appThreePeak,
                  uint32_t appOneNoise, uint32_t appTwoNoise, uint32_t appThreeNoise,
                  uint32_t appOneShift, uint32_t appTwoShift, uint32_t appThreeShift, StrategyTypes strategyType);

    uint32_t applicationOnePeak;

    uint32_t applicationTwoPeak;

    uint32_t applicationThreePeak;

    uint32_t applicationOneNoise;

    uint32_t applicationTwoNoise;

    uint32_t applicationThreeNoise;

    uint32_t applicationOneShift;

    uint32_t applicationTwoShift;

    uint32_t applicationThreeShift;

    StrategyTypes strategyType;
};

#endif // RUN_PARAMETERS_H