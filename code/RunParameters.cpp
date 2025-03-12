#include "RunParameters.h"
#include "StrategyTypes.h"

RunParameters::RunParameters(uint32_t appOnePeak, uint32_t appTwoPeak, uint32_t appThreePeak,
                             uint32_t appOneNoise, uint32_t appTwoNoise, uint32_t appThreeNoise,
                             uint32_t appOneShift, uint32_t appTwoShift, uint32_t appThreeShift, StrategyTypes strategyType)
    : applicationOnePeak(appOnePeak),
      applicationTwoPeak(appTwoPeak),
      applicationThreePeak(appThreePeak),
      applicationOneNoise(appOneNoise),
      applicationTwoNoise(appTwoNoise),
      applicationThreeNoise(appThreeNoise),
      applicationOneShift(appOneShift),
      applicationTwoShift(appTwoShift),
      applicationThreeShift(appThreeShift),
      strategyType(strategyType) {}