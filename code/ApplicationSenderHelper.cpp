#include "ApplicationSenderHelper.h"

#include <ns3/string.h>

namespace ns3
{

ApplicationSenderHelper::ApplicationSenderHelper(const std::string& protocol, const Address& address)
    : ApplicationHelper("SenderApplication")
{
    m_factory.Set("Protocol", StringValue(protocol));
    m_factory.Set("Remote", AddressValue(address));
}

} // namespace ns3
