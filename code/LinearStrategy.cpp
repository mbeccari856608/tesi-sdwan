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
    std::shared_ptr<SDWanApplication>& firstApplication = this->applications->at(0);
    ISPInterface& firstInterface = this->availableInterfaces.at(0);
    while (!firstApplication->pendingpackets.empty()){
        firstApplication->pendingpackets.pop();
        firstInterface.enqueuePacket();
        std::cout << "Accodato" << "\n";
    }

}