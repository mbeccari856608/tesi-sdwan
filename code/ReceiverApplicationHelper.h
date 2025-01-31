#ifndef RECEIVER_APPLICATION_HELPER_H
#define RECEIVER_APPLICATION_HELPER_H

#include <ns3/application-helper.h>
#include <memory>
#include "ISPInterface.h"
namespace ns3
{

  class ReceiverApplicationHelper : public ApplicationHelper
  {
  public:
    /**
     * Create a DestinationHelper to make it easier to work with ReceiverApplicaiton
     *
     * \param interfaces the ISP interfaces from which data can arrive
     *
     */
    ReceiverApplicationHelper(std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> interfaces);

    /**
     * Due to how ns3 works we may need to do some manual tweaking when adding the application
     * to the device.
     */
    Ptr<Application> DoInstall(Ptr<Node> node) override;

  private:
    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> interfaces;
  };

} // namespace ns3

#endif /* RECEIVER_APPLICATION_HELPER_H */
