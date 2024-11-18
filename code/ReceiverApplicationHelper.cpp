#include "ReceiverApplicationHelper.h"

#include <ns3/string.h>

namespace ns3
{

ReceiverApplicationHelper::ReceiverApplicationHelper(const Address& address)
    : ApplicationHelper("ReceiverApplication")
{
    
    m_factory.Set("Local", AddressValue(address));
}

} // namespace ns3
