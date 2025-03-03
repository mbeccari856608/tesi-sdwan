#ifndef COMPUTE_OPTIMIZATION_RESULT_H
#define COMPUTE_OPTIMIZATION_RESULT_H

#include "ns3/address.h"
#include "ns3/socket.h"
#include "ns3/error-model.h"
#include <cstdint>
#include <queue>
#include "ns3/internet-module.h"
#include "SendPacketInfo.h"
#include <optional>
#include "ISPInterface.h"

/**
 * @brief Class used to represent the result of computing the cost-minimization problem
 * for a set of application over a set of interfaces.
 *
 */
class ComputeOptimizationResult
{

private:
    bool _isFeasible;
    std::vector<std::vector<uint32_t>> _solutionValues;
    uint32_t _totalCost;

public:
    ComputeOptimizationResult(
        bool isFeasible,
        std::vector<std::vector<uint32_t>> resultValues,
        std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> interfaces,
        uint32_t totalCost);

    const uint32_t getTotalCost();

    /**
     * Whether or not the solution is feasible.
     *
     */
    const bool getIsFeasible();

    /**
     * Gets the computed solution, if available.
     *
     */
    const std::vector<std::vector<uint32_t>> &getSolutionValues() const;

    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> interfaces;
};

#endif // COMPUTE_OPTIMIZATION_RESULT_H