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
     * \param address the address of the sink,
     *
     */
    ReceiverApplicationHelper(const Address &address);
  };

} // namespace ns3

#endif /* RECEIVER_APPLICATION_HELPER_H */
