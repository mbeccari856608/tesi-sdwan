#ifndef ROUND_ROBIN_STRATEGY_H
#define ROUND_ROBIN_STRATEGY_H

#include <memory>
#include <vector>
#include <algorithm>
#include "Strategy.h"
#include "SDWanApplication.h"
#include "ISPInterface.h"
#include <random>

class RoundRobinStrategy : public Strategy
{

public:
    RoundRobinStrategy(
        std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
        std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> availableInterfaces);

    virtual void Compute();
};

#endif /* ROUND_ROBIN_STRATEGY_H */
