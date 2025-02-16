#include "RoundRobinStrategy.h"

RoundRobinStrategy::RoundRobinStrategy(
    std::shared_ptr<std::vector<std::shared_ptr<SDWanApplication>>> applications,
    std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> availableInterfaces)
    : Strategy(applications, availableInterfaces) 
{
}

void RoundRobinStrategy::Compute()
{

     uint32_t currentIndex = 0;
     uint32_t totalInterfacesAmount = availableInterfaces->size();
        
    for (size_t i = 0; i < applications->size(); i++)
    {
        auto currentApplication = applications->at(i);
        
        if (currentApplication->pendingpackets.empty())
        {
            continue;
        }

        ns3::Time currentTime = ns3::Simulator::Now();
        uint32_t applicationId = currentApplication->applicationId;

        while (!currentApplication->pendingpackets.empty())
        {
            currentApplication->pendingpackets.pop();
            SendPacketInfo packetInfo;

            packetInfo.dateEnqueued = currentTime;
            packetInfo.originatedFrom = applicationId;

            this->availableInterfaces->at(currentIndex)->enqueuePacket(packetInfo);
            currentIndex = (currentIndex + 1) % totalInterfacesAmount;
        }
    }
}