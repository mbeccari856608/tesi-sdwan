#ifndef SEND_PACKET_INFO_H
#define SEND_PACKET_INFO_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/simulator.h"


/**
 * This struct is used to gather the information
 * that must be added to a packet before sending it.
 * 
 */
struct SendPacketInfo
{
    ns3::Time dateEnqueued;

    uint32_t originatedFrom;
};



#endif // SEND_PACKET_INFO_H