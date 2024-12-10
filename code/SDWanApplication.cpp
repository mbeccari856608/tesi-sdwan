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
SDWanApplication::SDWanApplication() :
      requiredDataRate(),
      requiredDelay(),
      errorRate(),
      hasStoppedGeneratingData(false),
      pendingpackets() {}

SDWanApplication::SDWanApplication(
    ns3::DataRateValue requiredDataRate,
    uint32_t requiredDelay,
    uint32_t errorRate)
    : requiredDataRate(requiredDataRate),
      requiredDelay(requiredDelay),
      errorRate(errorRate),
      hasStoppedGeneratingData(false),
      pendingpackets() {}


SDWanApplication::SDWanApplication(const SDWanApplication &data) 
: requiredDataRate(data.requiredDataRate),
  errorRate(data.errorRate),
  requiredDelay(data.requiredDelay) {}

 SDWanApplication& SDWanApplication::operator=(const SDWanApplication &data) {
  return *this;
 }

 SDWanApplication::SDWanApplication(SDWanApplication &&data) 
 : requiredDataRate(data.requiredDataRate),
   errorRate(data.errorRate),
   requiredDelay(data.requiredDelay) {


 }

  SDWanApplication& SDWanApplication::operator=(SDWanApplication &&data) {
   return *this;
  }
