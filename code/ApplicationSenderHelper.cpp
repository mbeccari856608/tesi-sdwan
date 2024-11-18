#include "ApplicationSenderHelper.h"

#include <ns3/string.h>
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/object-vector.h"
#include "SenderApplication.h"

namespace ns3
{

  ApplicationSenderHelper::ApplicationSenderHelper(
      std::vector<Address> &addresses,
      uint32_t amount)
      : ApplicationHelper("SenderApplication"), addresses(addresses)
  {
    m_factory.Set("TotalAmount", UintegerValue(amount));
  }

  ns3::Ptr<Application> ApplicationSenderHelper::DoInstall(Ptr<Node> node)
  {
    ns3::Ptr<SenderApplication> baseApplication = DynamicCast<SenderApplication>(ApplicationHelper::DoInstall(node));
    baseApplication->addresses = std::make_unique<std::vector<Address>>(addresses);
    return baseApplication;
  }

} // namespace ns3
