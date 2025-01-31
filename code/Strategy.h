
#ifndef STRATEGY_H
#define STRATEGY_H
#include <memory>
#include <vector>
#include <algorithm>
#include "SDWanApplication.h"
#include "ISPInterface.h"

/**
 * @brief This class serves as a base for any strategy.
 *
 * With strategy we mean any kind of algorithm that may be used to decide
 * which data goes through each interface.
 */
class Strategy
{
public:
    virtual ~Strategy() = default;
    Strategy(
        std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
        std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> availableInterfaces);

    std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications;
    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> availableInterfaces;
    
    bool getAllDataHasBeenSent();

    virtual void Compute() = 0;

protected:
};

#endif /* STRATEGY_H */
