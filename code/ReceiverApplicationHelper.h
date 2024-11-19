#ifndef RECEIVER_APPLICATION_HELPER_H
#define RECEIVER_APPLICATION_HELPER_H

#include <ns3/application-helper.h>

namespace ns3
{

  class ReceiverApplicationHelper : public ApplicationHelper
  {
  public:
    /**
     * Create a DestinationHelper to make it easier to work with ReceiverApplicaiton
     *
     * \param address the addresses of the interfaces that can receive data
     *
     */
    ReceiverApplicationHelper(std::vector<ns3::Address> &sources);

    /**
     * Due to how ns3 works we may need to do some manual tweaking when adding the application
     * to the device.
     */
    Ptr<Application> DoInstall(Ptr<Node> node) override;

  private:
    std::vector<Address> &sources;
  };

} // namespace ns3

#endif /* RECEIVER_APPLICATION_HELPER_H */
