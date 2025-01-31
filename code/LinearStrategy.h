#ifndef LINEAR_STRATEGY_H
#define LINEAR_STRATEGY_H

#include <memory>
#include <vector>
#include <algorithm>
#include "Strategy.h"
#include "SDWanApplication.h"
#include "ISPInterface.h"

class LinearStrategy : public Strategy
{
private:
    /**
     * Linear strategy is computed only once
     */
    bool hasBeenComputed = false;
public:
    LinearStrategy(
        std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
        std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> availableInterfaces);

    virtual void Compute();
};



#endif /* LINEAR_STRATEGY_H */
