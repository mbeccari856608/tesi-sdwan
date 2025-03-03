#include <cstdint>
#include <bitset>
#include <algorithm>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "Utils.h"

ns3::DataRateValue Utils::ConvertPacketsPerSecondToBitPerSecond(uint32_t packetsPerSecond)
{
    return ns3::DataRateValue(ConvertPacketsPerSecondToBitPerSecondToDataRate(packetsPerSecond));
}

ns3::DataRate Utils::ConvertPacketsPerSecondToBitPerSecondToDataRate(uint32_t packetsPerSecond)
{
    return ns3::DataRate(packetsPerSecond * PacketSizeBit);
}

std::vector<bool> Utils::getBoolVectorFromInt(uint32_t num, uint32_t bit_size)
{
    std::bitset<32> bits(num);
    std::vector<bool> binaryArray;

    for (size_t i = 0; i < bit_size; ++i)
    {
        binaryArray.push_back(bits[i]);
    }
    std::reverse(binaryArray.begin(), binaryArray.end());
    return binaryArray;
}

uint32_t Utils::maxIntWithNBits(uint32_t n)
{
    if (n == 0)
        return 0;
    return (1U << n) - 1;
}
