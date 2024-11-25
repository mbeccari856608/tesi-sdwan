#ifndef ISP_INTERFACE_H
#define ISP_INTERFACE_H

#include "ns3/address.h"
#include "ns3/socket.h"
#include "ns3/error-model.h"

/**
 * @brief Class used to represent a network interface in an SD-WAN context.
 * 
 * This class contains informations used to send data through a particolar interface
 * and keep track on how many data is sent.
 */
class ISPInterface
{

    public:
    /**
     * @brief IP from where the data is sent from. 
     */
    const ns3::Address& outgoingAddress;

    /**
     * @brief Pointer to the socket used to send data.
     */
    const  ns3::Ptr<ns3::Socket>& socketInfo;

    bool connected;

    /**
     * The address to which data is sent
    */
    const ns3::Address& destinationAddress;

    /**
     * The object used do determine corrupt packages.
     */
    const ns3::RateErrorModel errorModel;

    uint32_t correctPackages;

    uint32_t corruptPackages;



};

#endif // ISP_INTERFACE_H