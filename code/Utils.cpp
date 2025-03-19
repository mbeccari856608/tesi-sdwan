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

void generateArrays(uint32_t k, uint32_t n, std::vector<uint32_t> &currentArray, std::vector<std::vector<uint32_t>> &result)
{
    if (currentArray.size() == k)
    {
        result.push_back(currentArray);
        return;
    }

    for (int i = 1; i <= n; ++i)
    {
        currentArray.push_back(i);
        generateArrays(k, n, currentArray, result);
        currentArray.pop_back();
    }
}

std::unique_ptr<std::vector<std::vector<uint32_t>>> Utils::getPermutationsWithRepetitionOfKelementsOfLengthN(uint32_t N, uint32_t k)
{
    std::vector<std::vector<uint32_t>> result;
    std::vector<uint32_t> currentArray;
    generateArrays(N, k, currentArray, result);
    return std::make_unique<std::vector<std::vector<uint32_t>>>(result);
}

void Utils::printResultsToFile(std::string fileName, std::vector<StrategyDataPoint> &dataPoints)
{
    std::ofstream file(fileName);

    if (!file)
    {
        throw std::runtime_error("Impossibile aprire il file");
    }

    file << "Index,CurrentCost,TotalCost,CurrentPackets,TotalPackets, TrafficIntensity" << std::endl;
    for (size_t i = 0; i < dataPoints.size(); ++i)
    {
        auto dataPoint = dataPoints.at(i);
        double trafficIntensity = (double)dataPoint.totalAmountOfPackets / (i + 1);
        file << i + 1 << ","
             << dataPoint.currentCost << ","
             << dataPoint.totalCost << ","
             << dataPoint.currentAmountOfPackets << ","
             << dataPoint.totalAmountOfPackets << ","
             << trafficIntensity
             << std::endl;
    }

    file.close();
}

void Utils::printResultsToFile(std::string fileName, std::vector<RunInfo> &results)
{
    std::ofstream file(fileName);

    if (!file)
    {
        throw std::runtime_error("Impossibile aprire il file");
    }

    file << "Run,"
         << "Application1AverageBandWidth,Application2AverageBandWidth,Application3AverageBandWidth,"
         << "Application1ErrorRate,Application2ErrorRate,Application3ErrorRate,"
         << "Application1Noise,Application2Noise,Application3Noise,"
         << "Application1Peak,Application2Peak,Application3Peak,"
         << "Application1Shift,Application2Shift,Application3Shift,"
         << "AveragePacketCost,TotalPackets,TotalCost,"
         << std::endl;
    for (size_t i = 0; i < results.size(); ++i)
    {
        auto result = results.at(i);
        file 
             << i + 1 << ","

             << result.applicationBandWidth.at(0) << ","
             << result.applicationBandWidth.at(1) << ","
             << result.applicationBandWidth.at(2) << ","

             << result.applicationErrorRates.at(0) << ","
             << result.applicationErrorRates.at(1) << ","
             << result.applicationErrorRates.at(2) << ","

             << result.parameters.applicationOneNoise << ","
             << result.parameters.applicationTwoNoise << ","
             << result.parameters.applicationThreeNoise << ","

             << result.parameters.applicationOnePeak << ","
             << result.parameters.applicationTwoPeak << ","
             << result.parameters.applicationThreePeak << ","

             << result.parameters.applicationOneShift << ","
             << result.parameters.applicationTwoShift << ","
             << result.parameters.applicationThreeShift << ","

             << result.getAveragePacketCost() << ","
             << result.totalAmountOfPackets << ","
             << result.totalCost

             << std::endl;
    }

    file.close();
}