#ifndef SD_WAN_APPLICATION_H
#define SD_WAN_APPLICATION_H

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include <queue>
#include <stdint.h>

/**
 * This class represents a set of requirement that must be met while sending data
 * through a set of interfaces.
 */
class SDWanApplication
{
public:
    virtual ~SDWanApplication();

    SDWanApplication();


    SDWanApplication(
        ns3::DataRateValue requiredDataRate,
        uint32_t requiredDelay,
        uint32_t errorRate);

    SDWanApplication(const SDWanApplication &data);
    SDWanApplication& operator=(const SDWanApplication &data);
    SDWanApplication(SDWanApplication &&data);
    SDWanApplication& operator=(SDWanApplication &&data);

    /**
     * @brief
     * The maximum average required data rate for all the packets sent for the application.
     */
    const ns3::DataRateValue requiredDataRate;

    /**
     * @brief
     * The maximum average required delay for all the packets sent for the application.
     */
    const uint32_t requiredDelay;
    
    virtual bool getHasStoppedGeneratingData() = 0;

    /**
     * @brief
     *
     * The queue containing an element for each packet waiting to be sent.
     *
     * The actual element in the queue is arbitrary: we only want to keep track of the amount of packets.
     *
     */
    std::queue<uint8_t> pendingpackets;

    /**
     * @brief The maximum required error rate, expressed as a percentage
     *
     * @example If the desired error rate must be 80% this property will be 80.
     */
    const uint32_t errorRate;

    virtual void OnApplicationStart() = 0;

    void Update()
    {
        if (!this->getHasStoppedGeneratingData())
        {
            this->OnUpdate();
        }
    }

protected:
    virtual void OnUpdate() = 0;
};

#endif /* SD_WAN_APPLICATION_H */
