#include "DeviceApplication.h"
#include "ns3/csma-helper.h"


void OnSocketSend(ns3::Ptr<ns3::Socket> socket, uint32_t amount)
{
    std::cout << "Yooo"  << "\n";
}

void OnConnectedSuccess(ns3::Ptr<ns3::Socket> socket){
        std::cout << "Yooo"  << "\n";

}

void OnConnectedFailure(ns3::Ptr<ns3::Socket> socket){
        std::cout << "Yooo"  << "\n";

}

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

    this->transimissionSocket->BindToNetDevice(csmaNetDevice);

    if (this->transimissionSocket == nullptr)
    {
        std::cout << "Socket di origine non valida" << " \n";
        return;
    }

    transimissionSocket->SetSendCallback(ns3::MakeCallback(&OnSocketSend));
    transimissionSocket->SetDataSentCallback(ns3::MakeCallback(&OnSocketSend));
    transimissionSocket->SetConnectCallback(ns3::MakeCallback(&OnConnectedSuccess), ns3::MakeCallback(&OnConnectedSuccess));

    auto bindResult = this->transimissionSocket->Bind();
    std::cout << "Risultato binding: " << bindResult << "\n";
    if (bindResult != 0){
        std::cout << "Errore nel binding"  << "\n";
    }

    auto connectionResult = this->transimissionSocket->Connect(destinationAddress);
    if (connectionResult != 0){
        ns3::Socket::SocketErrno err = this->transimissionSocket->GetErrno();
        std::cout << "Errore di connessione " << err << "\n";
    }

    ns3::Simulator::Schedule(ns3::Seconds(5.0), &DeviceApplication::SendPacket, this);
}

void DeviceApplication::SendPacket()
{
    ns3::Ptr<ns3::Packet> packet = ns3::Create<ns3::Packet>(100000);
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