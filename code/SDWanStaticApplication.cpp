#include "SDWanApplication.h"
#include "SDWanStaticApplication.h"

void SDWanStaticApplication::OnApplicationStart(){
    uint8_t amountOfPackets = this->amountOfPacketsToSend;
    for (size_t i = 0; i < amountOfPackets; i++)
    {
        this->pendingpackets.push(i);
    }
    this->allPacketsGenerated = true;
    
}
SDWanStaticApplication::SDWanStaticApplication(
        ns3::DataRate requiredDataRate,
        uint32_t requiredDelay,
        uint32_t errorRate,
        uint32_t amountOfPacketsToSend) : SDWanApplication(requiredDataRate, requiredDelay, errorRate), amountOfPacketsToSend(amountOfPacketsToSend) {

}

void SDWanStaticApplication::OnUpdate(){

}
bool SDWanStaticApplication::getHasStoppedGeneratingData(){
    return this->pendingpackets.empty() && this->allPacketsGenerated;
}
