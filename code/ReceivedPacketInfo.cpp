
#include "ReceivedPacketInfo.h"
#include "ns3/address.h"


ReceivedPacketInfo::ReceivedPacketInfo(ns3::Address fromAddress, uint32_t sentFrom, uint32_t delayInMiliseconds) :
    address(fromAddress),
    sentFrom(sentFrom),
    delayInMiliseconds(delayInMiliseconds){

}