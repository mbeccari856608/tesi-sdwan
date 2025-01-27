#include "ISPInterface.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "Utils.h"
#include "ns3/csma-module.h"
#include "ns3/error-model.h"
#include "ns3/simulator.h"

ISPInterface::ISPInterface(
    ns3::Ptr<ns3::NetDevice> netDevice,
    ns3::Address outgoingAddress,
    ns3::Ptr<ns3::Socket> socketInfo,
    ns3::Address destinationAddress,
    ns3::RateErrorModel errorModel,
    uint32_t cost) : outgoingAddress(outgoingAddress),
                     socketInfo(socketInfo),
                     destinationAddress(destinationAddress),
                     errorModel(errorModel),
                     corruptPackages(0),
                     correctPackages(0),
                     connected(false),
                     netDevice(netDevice),
                     pendingpackets(),
                     cost(cost)
{
}
ISPInterface::ISPInterface(const ISPInterface &other)
    : outgoingAddress(other.outgoingAddress),
      socketInfo(other.socketInfo),
      destinationAddress(other.destinationAddress),
      errorModel(other.errorModel),
      corruptPackages(other.corruptPackages),
      correctPackages(other.correctPackages),
      connected(other.connected),
      netDevice(other.netDevice),
      pendingpackets(other.pendingpackets),
      cost(other.cost)
{
    std::cout << "Copiato" << "\n";
}

ISPInterface &ISPInterface::operator=(const ISPInterface &other)
{
    if (this != &other)
    {
        outgoingAddress = other.outgoingAddress;
        socketInfo = other.socketInfo;
        destinationAddress = other.destinationAddress;
        errorModel = other.errorModel;
        corruptPackages = other.corruptPackages;
        correctPackages = other.correctPackages;
        connected = other.connected;
        netDevice = other.netDevice;
        pendingpackets = other.pendingpackets;
    }
    return *this;
}

ns3::DataRate ISPInterface::getDataRate()
{
    auto channel = this->netDevice->GetChannel();
    ns3::DataRateValue dataRateValue;
    channel->GetAttribute("DataRate", dataRateValue);
    return dataRateValue.Get();
}

const double ISPInterface::getErrorRate()
{
    ns3::DoubleValue errorRateValue;
    this->errorModel.GetAttribute("ErrorRate", errorRateValue);
    double value = errorRateValue.Get();
    return value * 100;
}

const uint32_t ISPInterface::getDelayInMilliseconds()
{
    auto channel = this->netDevice->GetChannel();
    ns3::Ptr<ns3::CsmaChannel> csmaChannel = ns3::DynamicCast<ns3::CsmaChannel>(channel);
    auto result = csmaChannel->GetDelay().GetMilliSeconds();
    return result;
}

void ISPInterface::enqueuePacket(SendPacketInfo packet)
{
    this->pendingpackets.push(packet);
}

bool ISPInterface::getHasAnyAvailablePackage()
{
    return !this->pendingpackets.empty();
}

SendPacketInfo ISPInterface::getNextPacket()
{

    // TODO cambiare in STD::optional e continuare (finire) con i tag
    if (this->getHasAnyAvailablePackage())
    {
        SendPacketInfo value = this->pendingpackets.front();
        this->pendingpackets.pop();
                            // Recupero del TimestampTag dal pacchetto
                ns3::TimestampTag receivedTag;
                if (value->PeekPacketTag(receivedTag))
                {
                    std::cout << "Timestamp alla generazione: " 
                              << receivedTag.GetTimestamp().GetNanoSeconds() 
                              << " ns" << std::endl;
                }
        return value;
    }
    return nullptr;
}

const uint64_t ISPInterface::getDataBitRate()
{
    return this->getDataRate().GetBitRate();
}

double ISPInterface::getAverageWaitingTimeInMilliseconds()
{
    // The current number of packets is based on the ammount of packets in the queue.
    uint32_t currentPackets = this->pendingpackets.size();

    double currentTime = ns3::Simulator::Now().GetSeconds();
    double totalAmountOfPackets = this->correctPackages + this->corruptPackages;
    double arrivalRate = totalAmountOfPackets / currentTime;



    return (double) currentPackets / arrivalRate;

}