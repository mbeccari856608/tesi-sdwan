#include <memory>
#include <vector>
#include <algorithm>
#include "Strategy.h"
#include "SDWanApplication.h"
#include "ISPInterface.h"



Strategy::Strategy(
    std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
    std::vector<ISPInterface> &availableInterfaces) : applications(applications), availableInterfaces(availableInterfaces)
{
}

bool Strategy::getAllDataHasBeenSent()
{

    return std::all_of(this->applications->begin(), this->applications->end(), [](const std::shared_ptr<SDWanApplication> &item)
                       { return item->getHasStoppedGeneratingData(); });
}
