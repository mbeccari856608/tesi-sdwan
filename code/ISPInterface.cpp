#include "ISPInterface.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "Utils.h"

ISPInterface::ISPInterface(
    ns3::Ptr<ns3::NetDevice> netDevice,
    ns3::Address outgoingAddress,
    ns3::Ptr<ns3::Socket> socketInfo,
    ns3::Address destinationAddress,
    ns3::RateErrorModel &errorModel) : outgoingAddress(outgoingAddress),
                                       socketInfo(socketInfo),
                                       destinationAddress(destinationAddress),
                                       errorModel(errorModel),
                                       corruptPackages(0),
                                       correctPackages(0),
                                       connected(false),
                                       netDevice(netDevice),
                                       pendingpackets()
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
      pendingpackets(other.pendingpackets)
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

void ISPInterface::enqueuePacket()
{
    ns3::Ptr<ns3::Packet> packet = ns3::Create<ns3::Packet>(Utils::PacketSizeBit);
    this->pendingpackets.push(packet);
}

bool ISPInterface::getHasAnyAvailablePackage()
{
    return !this->pendingpackets.empty();
}

ns3::Ptr<ns3::Packet> ISPInterface::getNextPacket()
{
    if (this->getHasAnyAvailablePackage())
    {
        ns3::Ptr<ns3::Packet> value = this->pendingpackets.front();
        this->pendingpackets.pop();
        std::cout << "pop " << "(" << this->pendingpackets.size() << ")" << "\n";
        return value;
    }
    return nullptr;
}