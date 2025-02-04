#ifndef RECEIVED_PACKET_INFO_H
#define RECEIVED_PACKET_INFO_H
#include <stdint.h>
#include "ns3/address.h"
#include "memory.h"
#include "ISPInterface.h"
#include "SDWanApplication.h"
#include <boost/range/algorithm/sort.hpp>

/**
 * This class is used to track information about packets received from a given application.
 * 
 */
class ReceivedPacketInfo{

public:
    ReceivedPacketInfo(
        std::shared_ptr<ISPInterface> interface,
        uint32_t fromApplication);

    std::shared_ptr<ISPInterface> fromInterface;
    uint32_t fromApplication;

    uint32_t getDelayInMilliSeconds() const ;

    uint32_t getDataRateInPacketPerSeconds() const ;
};



#endif // RECEIVED_PACKET_INFO_H
