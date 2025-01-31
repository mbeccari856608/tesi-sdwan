#include "ReceivedPacketInfo.h"
#include "ns3/address.h"
#include "memory"

ReceivedPacketInfo::ReceivedPacketInfo(
        std::shared_ptr<ISPInterface> interface,
        uint32_t fromApplication) :
    fromInterface(interface),
    fromApplication(fromApplication) {

}