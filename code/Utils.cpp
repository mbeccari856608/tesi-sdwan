#include <cstdint>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "Utils.h"


    ns3::DataRateValue Utils::ConvertPacketsPerSecondToBitPerSecond(uint32_t packetsPerSecond)
    {
        return ns3::DataRateValue(ns3::DataRate(packetsPerSecond * PacketSizeBit));
    }
