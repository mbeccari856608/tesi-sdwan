#ifndef RECEIVED_PACKET_INFO_H
#define RECEIVED_PACKET_INFO_H
#include <stdint.h>
#include "ns3/address.h"
/**
 * This class is used to track information about packets received from a given application.
 * 
 */
class ReceivedPacketInfo{

public:
    ReceivedPacketInfo(
        ns3::Address fromAddress,
        uint32_t sentFrom,
        uint32_t delayInMiliseconds);


    /**
     * The address of the interface from which the packet was received
     * 
     */
    ns3::Address address;

    /**
     * Identifier used to track the application that created the packet.
     * 
     */
    uint32_t sentFrom;

    /**
     * Total delay experienced by the packet, calcolated from the time it was first queued.
     */
    uint32_t delayInMiliseconds;

};



#endif // RECEIVED_PACKET_INFO_H
