#include "SDWanApplication.h"
#include "Utils.h"
#include "SDWanStaticApplication.h"
#include "ns3/flow-id-tag.h"
#include "ns3/timestamp-tag.h"

void SDWanStaticApplication::OnApplicationStart()
{
    uint8_t amountOfPackets = this->amountOfPacketsToSend;
    for (size_t i = 0; i < amountOfPackets; i++)
    {
        this->pendingpackets.push(i);
        this->generatedPackets++;
    }
    this->allPacketsGenerated = true;
}
SDWanStaticApplication::SDWanStaticApplication(
    uint32_t applicationId,
    ns3::DataRate requiredDataRate,
    uint32_t requiredDelay,
    uint32_t errorRate,
    uint32_t amountOfPacketsToSend) : SDWanApplication(applicationId, requiredDataRate, requiredDelay, errorRate), amountOfPacketsToSend(amountOfPacketsToSend)
{
}

void SDWanStaticApplication::OnUpdate()
{
}
bool SDWanStaticApplication::getHasStoppedGeneratingData()
{
    return this->pendingpackets.empty() && this->allPacketsGenerated;
}
