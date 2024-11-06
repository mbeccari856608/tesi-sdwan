#include "DeviceApplication.h"
#include "ns3/csma-helper.h"


DeviceApplication::DeviceApplication() :
                                         destinationAddress()
{
}

DeviceApplication::~DeviceApplication()
{
}

void DeviceApplication::StartApplication()
{
}

void DeviceApplication::StopApplication()
{
}

void DeviceApplication::Setup(ns3::Address address)
{
    destinationAddress = address;

    std::cout << "Setup sul nodo" << "\n";
    ns3::Ptr<ns3::Node> currentNode = this->GetNode();
    if (currentNode == nullptr)
    {
        std::cout << "Errore" << "\n";
        return;
    }

    uint32_t amountOfDevices = currentNode->GetNDevices();
    std::cout << "Sono stati trovati " << amountOfDevices << " dispositivi" << "\n";
    ns3::Ptr<ns3::NetDevice> csmaNetDevice = currentNode->GetDevice(0);
    if (csmaNetDevice == nullptr)
    {
        std::cout << "Errore" << "\n";
        return;
    }
    else
    {
        std::cout << "Interfacce trovata" << "\n";
    }


    ns3::Simulator::Schedule(ns3::Seconds(5.0), &DeviceApplication::SendPacket, this);
}

void DeviceApplication::SendPacket()
{
        ns3::Ptr<ns3::Packet> packet = ns3::Create<ns3::Packet>(128);

    ns3::Ptr<ns3::NetDevice> csmaNetDevice = this->GetNode()->GetDevice(0);
    std::cout << "Pacchetto mandato" << " \n";
}

ns3::TypeId
DeviceApplication::GetTypeId()
{
    static ns3::TypeId tid = ns3::TypeId("DeviceApplication")
                                 .SetParent<ns3::Application>()
                                 .SetGroupName("SDWAN_CPE")
                                 .AddConstructor<DeviceApplication>();
    return tid;
}