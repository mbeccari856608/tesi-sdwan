#include "ApplicationSenderHelper.h"

#include <ns3/string.h>
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/object-vector.h"
#include "SenderApplication.h"
#include "Strategy.h"
#include "StrategyTypes.h"

using namespace ns3;

ApplicationSenderHelper::ApplicationSenderHelper(
    std::vector<Address> &addresses,
    std::vector<std::shared_ptr<SDWanApplication>> &applications,
    std::vector<uint32_t>& costs,
    StrategyTypes strategy)
    : ApplicationHelper("SenderApplication"),
      addresses(addresses),
      applications(applications),
      strategy(strategy),
      costs(costs)
{
}

ns3::Ptr<Application> ApplicationSenderHelper::DoInstall(Ptr<Node> node)
{
  ns3::Ptr<SenderApplication> baseApplication = DynamicCast<SenderApplication>(ApplicationHelper::DoInstall(node));
  baseApplication->addresses = std::make_unique<std::vector<Address>>(addresses);
  baseApplication->application = std::make_unique<std::vector<std::shared_ptr<SDWanApplication>>>(applications);
  baseApplication->costs = std::make_unique<std::vector<uint32_t>>(costs);
  baseApplication->strategyType = this->strategy;
  return baseApplication;
}
