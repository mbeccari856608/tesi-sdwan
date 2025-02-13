#include <cstdint>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "Utils.h"


    ns3::DataRateValue Utils::ConvertPacketsPerSecondToBitPerSecond(uint32_t packetsPerSecond)
    {
        return ns3::DataRateValue(ConvertPacketsPerSecondToBitPerSecondToDataRate(packetsPerSecond));
    }


    ns3::DataRate Utils::ConvertPacketsPerSecondToBitPerSecondToDataRate(uint32_t packetsPerSecond){
        return ns3::DataRate(packetsPerSecond * PacketSizeBit);
    }