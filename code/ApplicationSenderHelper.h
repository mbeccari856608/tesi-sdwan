#ifndef APPLICATION_SENDER_HELPER_H
#define APPLICATION_SENDER_HELPER_H

#include <ns3/application-helper.h>

namespace ns3
{


class ApplicationSenderHelper : public ApplicationHelper
{
  public:
    /**
     * Create an ApplicationSenderHelper to make it easier to work with BulkSendApplications
     *
     * \param address the address of the remote node to send traffic
     *        to.
     */
    ApplicationSenderHelper(const Address& local, const Address& address, uint32_t amount);
};

} // namespace ns3

#endif /* APPLICATION_SENDER_HELPER_H */
