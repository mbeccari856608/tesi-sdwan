#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/csma-module.h"
#include "SDWanApplication.h"
#include "ns3/point-to-point-module.h"

SDWanApplication::~SDWanApplication()
{
}
SDWanApplication::SDWanApplication() : applicationId(),
                                       requiredDelay(),
                                       errorRate(),
                                       pendingpackets(),
                                       generatedPackets(0) {}

SDWanApplication::SDWanApplication(
    uint32_t id,
    uint32_t requiredDelay,
    uint32_t errorRate) :
      applicationId(id),
      requiredDelay(requiredDelay),
      errorRate(errorRate),
      pendingpackets(),
      generatedPackets(0) {}

SDWanApplication::SDWanApplication(const SDWanApplication &data)
    :
      errorRate(data.errorRate),
      applicationId(data.applicationId),
      requiredDelay(data.requiredDelay),
      pendingpackets(data.pendingpackets),
      generatedPackets(data.generatedPackets) {}

SDWanApplication &SDWanApplication::operator=(const SDWanApplication &data)
{
  return *this;
}

SDWanApplication::SDWanApplication(SDWanApplication &&data)   :
      errorRate(data.errorRate),
      applicationId(data.applicationId),
      requiredDelay(data.requiredDelay),
      pendingpackets(data.pendingpackets),
      generatedPackets(data.generatedPackets)
{
}

SDWanApplication &SDWanApplication::operator=(SDWanApplication &&data)
{
  return *this;
}
