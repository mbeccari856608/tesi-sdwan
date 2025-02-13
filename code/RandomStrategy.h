#ifndef RANDOM_STRATEGY_H
#define RANDOM_STRATEGY_H

#include <memory>
#include <vector>
#include <algorithm>
#include "Strategy.h"
#include "SDWanApplication.h"
#include "ISPInterface.h"
#include <random>

class RandomStrategy : public Strategy
{

public:
    RandomStrategy(
        std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
        std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> availableInterfaces);

    virtual void Compute();

private:
    /**
     * Object used to generate randomness for the algorithm
     */
    std::default_random_engine rng;
};

#endif /* RANDOM_STRATEGY_H */
