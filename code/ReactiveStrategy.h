#ifndef REACTIVE_STRATEGY_H
#define REACTIVE_STRATEGY_H

#include <memory>
#include <vector>
#include <algorithm>
#include <stdint.h>

#include "Strategy.h"
#include "SDWanApplication.h"
#include "ComputeOptimizationResult.h"
#include "ISPInterface.h"
#include <random>

/**
 * @brief Class representing the core strategy of the paper.
 */
class ReactiveStrategy : public Strategy
{

public:
    ReactiveStrategy(
        std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
        std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> availableInterfaces);

    virtual void Compute();

private:
    std::vector<uint32_t> costs;

    std::map<std::shared_ptr<SDWanApplication>, std::vector<std::shared_ptr<ISPInterface>>> applicationToInterfacesMap;

    /**
     * @brief Computes the linear optimization problem for a given subset of applications across all the available interfaces.
     *
     *
     */
    std::shared_ptr<ComputeOptimizationResult> ComputeOptimization(std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> currentApplications,
                                                                   std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> currentInterfaces);

    uint32_t DelayEstimator(
        std::shared_ptr<SDWanApplication> application,
        std::shared_ptr<ISPInterface> interface);
};

#endif /* REACTIVE_STRATEGY_H */
