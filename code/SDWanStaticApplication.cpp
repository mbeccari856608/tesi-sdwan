#include "SDWanApplication.h"
#include "SDWanStaticApplication.h"

void SDWanStaticApplication::OnApplicationStart(){
    uint8_t amountOfPackets = 3;
    for (size_t i = 0; i < amountOfPackets; i++)
    {
        this->pendingpackets.push(i);
    }
    this->allPacketsGenerated = true;
    
}
SDWanStaticApplication::SDWanStaticApplication(
        ns3::DataRateValue requiredDataRate,
        uint32_t requiredDelay,
        uint32_t errorRate) : SDWanApplication(requiredDataRate, requiredDelay, errorRate) {

}

void SDWanStaticApplication::OnUpdate(){

}
bool SDWanStaticApplication::getHasStoppedGeneratingData(){
    return this->pendingpackets.empty() && this->allPacketsGenerated;
}
