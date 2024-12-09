#include "ApplicationSenderHelper.h"

#include <ns3/string.h>
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/object-vector.h"
#include "SenderApplication.h"

using namespace ns3;

ApplicationSenderHelper::ApplicationSenderHelper(
    std::vector<Address> &addresses,
    std::vector<SDWanApplication> &applications)
    : ApplicationHelper("SenderApplication"), addresses(addresses), applications(applications)
{
}

ns3::Ptr<Application> ApplicationSenderHelper::DoInstall(Ptr<Node> node)
{
  ns3::Ptr<SenderApplication> baseApplication = DynamicCast<SenderApplication>(ApplicationHelper::DoInstall(node));
  baseApplication->addresses = std::make_unique<std::vector<Address>>(addresses);
  baseApplication->application = std::make_unique<std::vector<SDWanApplication>>(applications);
  return baseApplication;
}
