#include "ApplicationSenderHelper.h"

#include <ns3/string.h>
#include "ns3/uinteger.h"

namespace ns3
{

ApplicationSenderHelper::ApplicationSenderHelper(
  const Address& local,
  const Address& address,
  uint32_t amount)
    : ApplicationHelper("SenderApplication")
{
    m_factory.Set("Local", AddressValue(address));
    m_factory.Set("TotalAmount", UintegerValue(amount));
    m_factory.Set("Remote", AddressValue(address));
}

} // namespace ns3
