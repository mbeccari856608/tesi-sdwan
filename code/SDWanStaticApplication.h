#ifndef SD_WAN_STATIC_APPLICATION_H
#define SD_WAN_STATIC_APPLICATION_H

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/csma-module.h"
#include "SDWanApplication.h"
#include "ns3/point-to-point-module.h"

/**
 * Extension of {@link SdWanApplication } used to represent an application
 * that just generates data at the beginning of its lifecycle, and it then stops generating data.
 */
class SDWanStaticApplication : public SDWanApplication
{
    public:

    SDWanStaticApplication();
    
    SDWanStaticApplication(
        ns3::DataRateValue requiredDataRate,
        uint32_t requiredDelay,
        uint32_t errorRate);
        
    virtual void OnUpdate();
    
    protected:
        virtual void OnApplicationStart();
};

#endif /* SD_WAN_STATIC_APPLICATION_H */
