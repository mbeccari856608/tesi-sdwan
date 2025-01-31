#include "ReceiverApplicationHelper.h"
#include "ReceiverApplication.h"
#include <ns3/string.h>

namespace ns3
{

  ReceiverApplicationHelper::ReceiverApplicationHelper(std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> interfaces)
      : ApplicationHelper("ReceiverApplication"), interfaces(interfaces)
  {
  }

  ns3::Ptr<Application> ReceiverApplicationHelper::DoInstall(Ptr<Node> node)
  {
    ns3::Ptr<ReceiverApplication> baseApplication = DynamicCast<ReceiverApplication>(ApplicationHelper::DoInstall(node));
    baseApplication->interfaces = this->interfaces;
    return baseApplication;
  }

} // namespace ns3
