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
     * \param protocol the name of the protocol to use to send traffic
     *        by the applications. This string identifies the socket
     *        factory type used to create sockets for the applications.
     *        A typical value would be ns3::TcpSocketFactory.
     * \param address the address of the remote node to send traffic
     *        to.
     */
    ApplicationSenderHelper(const std::string& protocol, const Address& address);
};

} // namespace ns3

#endif /* APPLICATION_SENDER_HELPER_H */
