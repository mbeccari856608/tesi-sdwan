#include "ReceiverApplicationHelper.h"
#include "ReceiverApplication.h"
#include <ns3/string.h>

namespace ns3
{

ReceiverApplicationHelper::ReceiverApplicationHelper(std::vector<ns3::Address>& sources)
    : ApplicationHelper("ReceiverApplication"), sources(sources)
{
}


  ns3::Ptr<Application> ReceiverApplicationHelper::DoInstall(Ptr<Node> node)
  {
    ns3::Ptr<ReceiverApplication> baseApplication = DynamicCast<ReceiverApplication>(ApplicationHelper::DoInstall(node));
    baseApplication->sources = std::make_unique<std::vector<Address>>(this->sources);
    return baseApplication;
  }

} // namespace ns3
