#include "ComputeOptimizationResult.h"
#include "ISPInterface.h"
ComputeOptimizationResult::ComputeOptimizationResult(
    bool isFeasible,
    std::vector<std::vector<uint32_t>> solutionValues,
    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> interfaces,
    uint32_t totalCost) : _isFeasible(isFeasible), _solutionValues(solutionValues), _totalCost(totalCost), interfaces(interfaces)
{
}

const bool ComputeOptimizationResult::getIsFeasible()
{
    return _isFeasible;
}

const std::vector<std::vector<uint32_t>> &ComputeOptimizationResult::getSolutionValues() const
{
    return this->_solutionValues;
}

const uint32_t ComputeOptimizationResult::getTotalCost()
{
    return this->_totalCost;
}