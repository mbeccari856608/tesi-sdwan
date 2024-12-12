#include <memory>
#include <vector>
#include <algorithm>
#include "Strategy.h"
#include "LinearStrategy.h"
#include "SDWanApplication.h"
#include "ISPInterface.h"

LinearStrategy::LinearStrategy(
        std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
        std::vector<ISPInterface> &availableInterfaces) :
             Strategy(applications, availableInterfaces)
{
}

void LinearStrategy::Compute()
{
    auto firstApplication = this->applications->at(0);
    auto firstInterface = this->availableInterfaces.at(0);
    if (!firstApplication->pendingpackets.empty()){
        firstApplication->pendingpackets.pop();
        firstInterface.enqueuePacket();
    }

}