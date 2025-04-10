#ifndef ISP_INTERFACE_H
#define ISP_INTERFACE_H

#include "ns3/address.h"
#include "ns3/socket.h"
#include "ns3/error-model.h"
#include <cstdint>
#include <queue>
#include "ns3/internet-module.h"
#include "SendPacketInfo.h"
#include <optional>
/**
 * @brief Class used to represent a network interface in an SD-WAN context.
 *
 * This class contains informations used to send data through a particolar interface
 * and keep track on how many data is sent.
 */
class ISPInterface
{

public:
    ISPInterface(
        uint32_t interfaceId,
        std::string interfaceName,
        ns3::Ptr<ns3::NetDevice> netDevice,
        ns3::Address outgoingAddress,
        ns3::Ptr<ns3::Socket> socketInfo,
        ns3::Address destinationAddress,
        ns3::RateErrorModel errorModel,
        uint32_t cost);

    ISPInterface(const ISPInterface &other);

    ISPInterface &operator=(const ISPInterface &other);

    /**
     * @brief Pointer to the net device associated with the interface.
     */
    ns3::Ptr<ns3::NetDevice> netDevice;

    /**
     * @brief IP from where the data is sent from.
     */
    ns3::Address outgoingAddress;

    /**
     * @brief Pointer to the socket used to send data.
     */
    ns3::Ptr<ns3::Socket> socketInfo;

    bool connected;

    /**
     * The address to which data is sent
     */
    ns3::Address destinationAddress;

    /**
     * The object used do determine whether or not a package is corrupt.
     */
    ns3::RateErrorModel errorModel;

    /**
     * Gets the maximum data rate this interface can send data with.
     */
    ns3::DataRate getDataRate();

    /**
     * Gets the maximum rate this interface can send data with, expressed in bit per second.
     */
    const uint64_t getDataBitRate();

    /**
     * Gets the maximum rate this interface can send data with, expressed in packages per second.
     */
    const uint64_t getPackageRate();

    /**
     * Gets the delay of the interface when seding packets.
     */
    const uint32_t getDelayInMilliseconds();

    /**
     * @brief Gets the error rate of this interface.
     * 
     * Example: If this value is 3, the error rate is 3%
     */
    const double getErrorRate();

    uint32_t correctPackages;

    uint32_t corruptPackages;

    void enqueuePacket(SendPacketInfo packetInfo);

    bool getHasAnyAvailablePackage();

    /**
     * @returns A pair which a value indicating whether there is a next package
     * and the info about the package.
     */
    std::pair<bool, SendPacketInfo> getNextPacket();

    /**
     * @brief Gets the average waiting time for a packet before it is sent through the interface.
     * This delay is calculated using the number of packets in the sistem divided by the average arrival
     * rate of new packages.
     * 
     */
    double getAverageWaitingTimeInMilliseconds();

    std::string interfaceName;

    uint32_t interfaceId;

    uint32_t cost;

private:
    /**
     * @brief Queue containing the packets waiting to be sent through this interface.
     */
    std::queue<SendPacketInfo> pendingpackets;

};

#endif // ISP_INTERFACE_H