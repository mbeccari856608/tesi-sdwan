#include "ReceiverApplicationHelper.h"

#include <ns3/string.h>

namespace ns3
{

ReceiverApplicationHelper::ReceiverApplicationHelper(const std::string& protocol, const Address& address)
    : ApplicationHelper("ReceiverApplication")
{
    m_factory.Set("Protocol", StringValue(protocol));
    m_factory.Set("Local", AddressValue(address));
}

} // namespace ns3
