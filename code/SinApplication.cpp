#include "SinApplication.h"
#include "SinGenerator.h"
#include "Utils.h"
#include <iomanip>

SinApplication::SinApplication() : SDWanApplication(), currentSample(0), allPacketsGenerated(false)
{
}

SinApplication::SinApplication(
    uint32_t id,
    uint32_t requiredDelay,
    uint32_t errorRate,
    uint32_t shift,
    uint32_t noise,
    uint32_t horizontalShift) : SDWanApplication(id, requiredDelay, errorRate),
                      currentSample(0),
                      allPacketsGenerated(false), shift(shift), noise(noise)
{
    this->sinValues = generateValues(shift, noise, horizontalShift);
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
    std::string applicationFileName =
        "sin_application_" +
        std::to_string(this->applicationId) +
        "_noise_" +
        std::to_string(this->noise) +
        "_shift_" +
        std::to_string(this->shift) + "_.csv";

    std::ofstream file(applicationFileName);

    // Check if the file is open
    if (!file.is_open())
    {
        throw std::runtime_error("Error opening file");
    }

    // Write the header
    file << "x,y\n";

    for (const auto &[x, y] : this->sinValues)
    {
        file << std::fixed << std::setprecision(6) << x << "," << y << "\n";
    }

    this->enqueuePacketsForCurrentSample();

    // Close the file
    file.close();
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