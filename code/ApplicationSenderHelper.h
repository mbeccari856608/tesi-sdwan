#ifndef APPLICATION_SENDER_HELPER_H
#define APPLICATION_SENDER_HELPER_H

#include <ns3/application-helper.h>

namespace ns3
{

// TODO rinominare
class ApplicationSenderHelper : public ApplicationHelper
{
  public:

    /**
     * Create an ApplicationSenderHelper to make it easier to work with SenderApplications.
     * 
     * @param addresses The possible destination addresses: the first address is for the first interface,
     *                  the second one is for the second interface and so on.
     *
     */
    ApplicationSenderHelper(std::vector<ns3::Address>& destinations, uint32_t packetAmount);

    /**
     * Due to how ns3 works we may need to do some manual tweaking when adding the application
     * to the device.
     */
    Ptr<Application> DoInstall(Ptr<Node> node) override;

    private:
      std::vector<Address>& addresses;
};

} // namespace ns3

#endif /* APPLICATION_SENDER_HELPER_H */
