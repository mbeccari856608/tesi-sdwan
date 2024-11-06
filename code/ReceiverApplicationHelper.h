#ifndef RECEIVER_APPLICATION_HELPER_H
#define RECEIVER_APPLICATION_HELPER_H

#include <ns3/application-helper.h>

namespace ns3
{


class ReceiverApplicationHelper : public ApplicationHelper
{
  public:
    /**
     * Create a PacketSinkHelper to make it easier to work with PacketSinkApplications
     *
     * \param protocol the name of the protocol to use to receive traffic
     *        This string identifies the socket factory type used to create
     *        sockets for the applications.  A typical value would be
     *        ns3::TcpSocketFactory.
     * \param address the address of the sink,
     *
     */
    ReceiverApplicationHelper(const std::string& protocol, const Address& address);
};

} // namespace ns3

#endif /* RECEIVER_APPLICATION_HELPER_H */
