// Utils.h

#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"

namespace Utils
{
    /**
     * Fixed sizes of a packet, in bits.
     */
    const uint32_t PacketSizeBit = 128;

    /**
     * @brief Converts a rate defined in packet per second to a rate in bits per second,
     * understandable by ns3 components.
     *
     * @param packetsPerSecond The rate in packets per second.
     * @return ns3::DataRateValue The rate in bits per second.
     */
    ns3::DataRateValue ConvertPacketsPerSecondToBitPerSecond(uint32_t packetsPerSecond);
}

#endif // UTILS_H