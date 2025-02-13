#include "SinApplication.h"
#include "SinGenerator.h"
#include "Utils.h"

SinApplication::SinApplication() : SDWanApplication(), currentSample(0), allPacketsGenerated(false)
{
}

SinApplication::SinApplication(
    uint32_t id,
    uint32_t requiredDelay,
    uint32_t errorRate,
    double shift) : SDWanApplication(id, requiredDelay, errorRate),
                    currentSample(0),
                    allPacketsGenerated(false)
{
    this->sinValues = generateValues(shift, 1);
}

void SinApplication::OnUpdate()
{
    this->enqueuePacketsForCurrentSample();
}

ns3::DataRate SinApplication::getRequiredDataRate()
{
    auto maxTuple = *std::max_element(this->sinValues.begin(), this->sinValues.end(),
                                      [](const auto &a, const auto &b)
                                      {
                                          return std::get<1>(a) < std::get<1>(b);
                                      });
    uint32_t maxValue = std::get<1>(maxTuple);
    return Utils::ConvertPacketsPerSecondToBitPerSecondToDataRate(maxValue);
}

bool SinApplication::getHasStoppedGeneratingData()
{
    return this->pendingpackets.empty() && this->allPacketsGenerated;
}

void SinApplication::OnApplicationStart()
{
    this->enqueuePacketsForCurrentSample();
}

void SinApplication::enqueuePacketsForCurrentSample()
{
    if (this->allPacketsGenerated)
    {
        return;
    }
    auto packetsStart = this->sinValues.at(this->currentSample);
    auto amountOfPackets = std::get<1>(packetsStart);

    for (size_t i = 0; i < amountOfPackets; i++)
    {
        this->pendingpackets.push(i);
        this->generatedPackets++;
    }

    this->currentSample++;
    if (this->currentSample == Utils::SimulationDurationInSeconds)
    {
        this->allPacketsGenerated = true;
    }
}