#ifndef SIN_APPLICATION_H
#define SIN_APPLICATION_H

#include <cstdint>
#include "SDWanApplication.h"
#include <vector>

/**
 * @brief Extension of {@link SdWanApplication} used to represent an application
 * that just generates data in a sinusoidal pattern thoughout its lifetime.
 */
class SinApplication : public SDWanApplication
{
public:
    SinApplication();

    SinApplication(
        uint32_t id,
        uint32_t requiredDelay,
        uint32_t errorRate,
        double shift);

    virtual void OnUpdate();
    virtual bool getHasStoppedGeneratingData();
    virtual ns3::DataRate getRequiredDataRate();

protected:
    virtual void OnApplicationStart();

private:
    bool allPacketsGenerated;
    std::vector<std::tuple<uint32_t, uint32_t>> sinValues;
    uint32_t currentSample;
    void enqueuePacketsForCurrentSample();
};




#endif // SIN_APPLICATION_H