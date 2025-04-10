// Utils.h

#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "StrategyDataPoint.h"
#include "RunInfo.h"

namespace Utils
{
    /**
     * Fixed sizes of a packet, in bits.
     */
    const uint32_t PacketSizeBit = 1024;

    /**
     * Fixed sizes of a packet, in bytes.
     */
    constexpr uint32_t PacketSizeByte = PacketSizeBit / 8;


    const uint32_t SeedForRandomGeneration = 1;

    const uint32_t SeedForNoiseGeneration = 19;

    const uint32_t SimulationDurationInSeconds = 1200;


    /**
     *  The port used to create sockets
     */
    const uint32_t ConnectionPort = 8080;

    /**
     * @brief Converts a rate defined in packet per second to a rate in bits per second,
     * understandable by ns3 components.
     *
     * @param packetsPerSecond The rate in packets per second.
     * @return ns3::DataRateValue The rate in bits per second.
     */
    ns3::DataRateValue ConvertPacketsPerSecondToBitPerSecond(uint32_t packetsPerSecond);


    ns3::DataRate ConvertPacketsPerSecondToBitPerSecondToDataRate(uint32_t packetsPerSecond);


    /**
     * Created a vector built from the binary represntation
     * of the input.
     * 
     * @example if the input is 13, its binary representation is 1101
     * This method will return am array containing:
     * true, true, false, true
     * 
     */
    std::vector<bool> getBoolVectorFromInt(uint32_t num, uint32_t bit_size);


    uint32_t maxIntWithNBits(uint32_t n);

    void printResultsToFile(std::string fileName, std::vector<StrategyDataPoint>& dataPoints);

    void printResultsToFile(std::string fileName, std::vector<RunInfo>& results);

    std::unique_ptr<std::vector<std::vector<uint32_t>>> getPermutationsWithRepetitionOfKelementsOfLengthN(uint32_t N, uint32_t k);



    template <typename K, typename V>
    typename std::map<K, V>::const_iterator getNextElement(
        const std::map<K, V> &m,
        typename std::map<K, V>::const_iterator current)
    {
        if (m.empty())
            return m.end();

        if (current == m.end() || current == nullptr)
        {
            return m.begin();
        }

        auto next = std::next(current);
        return (next == m.end()) ? m.begin() : next;
    }
}

#endif // UTILS_H