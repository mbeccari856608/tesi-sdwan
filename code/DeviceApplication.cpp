#include "DeviceApplication.h"
#include "ns3/csma-helper.h"

DeviceApplication::DeviceApplication() : transimissionSocket(nullptr),
                                         destinationAddress()
{
}

DeviceApplication::~DeviceApplication()
{
    transimissionSocket = nullptr;
}

void DeviceApplication::StartApplication()
{
}

void DeviceApplication::StopApplication()
{
    if (transimissionSocket)
    {
        transimissionSocket->Close();
    }
}

void DeviceApplication::Setup(ns3::Ptr<ns3::Socket> socket, ns3::Address address)
{
    transimissionSocket = socket;
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

    auto bindResult = this->transimissionSocket->Bind();
    std::cout << "Risultato binding: " << bindResult << "\n";

    if (this->transimissionSocket == nullptr)
    {
        std::cout << "Socket di destinazione non valida" << " \n";
        return;
    }

    auto connectResult = this->transimissionSocket->Connect(this->destinationAddress);
    std::cout << "Risultato connessione: " << connectResult << "\n";
    ns3::Socket::SocketErrno err = this->transimissionSocket->GetErrno();

    std::cout << "Errore: " << err << "\n";

    ns3::Simulator::Schedule(ns3::Seconds(2.0), &DeviceApplication::SendPacket, this);
}

void DeviceApplication::SendPacket()
{
    ns3::Ptr<ns3::Packet> packet = ns3::Create<ns3::Packet>(128);
    auto result = transimissionSocket->Send(packet);
    std::cout << "Risultato " << result << " \n";
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