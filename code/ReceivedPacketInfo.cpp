#include "ReceivedPacketInfo.h"
#include "ns3/address.h"
#include "memory"
#include "Utils.h"

ReceivedPacketInfo::ReceivedPacketInfo(
    std::shared_ptr<ISPInterface> interface,
    uint32_t fromApplication) : fromInterface(interface),
                                fromApplication(fromApplication)
{
}

uint32_t ReceivedPacketInfo::getDelayInMilliSeconds() const
{
    return this->fromInterface->getDelayInMilliseconds();
}

uint32_t ReceivedPacketInfo::getDataRateInPacketPerSeconds() const
{
    return this->fromInterface->getDataBitRate() / Utils::PacketSizeBit;
}