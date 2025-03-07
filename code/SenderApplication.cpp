#include "SenderApplication.h"

#include "ns3/address.h"
#include "ns3/ipv4.h"
#include "ns3/boolean.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/nstime.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/socket.h"
#include "ns3/tcp-socket-factory.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/uinteger.h"
#include "Utils.h"
#include "Strategy.h"
#include "StrategyTypes.h"
#include "LinearStrategy.h"
#include "RandomStrategy.h"
#include "RoundRobinStrategy.h"
#include "ReactiveStrategy.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SenderApplication");

NS_OBJECT_ENSURE_REGISTERED(SenderApplication);

uint32_t initialInterfaceId = 100;

uint32_t getNextInterfaceId()
{
    initialInterfaceId++;
    return initialInterfaceId;
}

TypeId
SenderApplication::GetTypeId()
{
    static TypeId tid =
        TypeId("SenderApplication")
            .SetParent<Application>()
            .SetGroupName("Applications")
            .AddConstructor<SenderApplication>()
            .AddAttribute("Tos",
                          "The Type of Service used to send IPv4 packets. "
                          "All 8 bits of the TOS byte are set (including ECN bits).",
                          UintegerValue(0),
                          MakeUintegerAccessor(&SenderApplication::m_tos),
                          MakeUintegerChecker<uint8_t>())
            .AddAttribute("TotalAmount",
                          "The total amount of bytes to send",
                          UintegerValue(128),
                          MakeUintegerAccessor(&SenderApplication::amountOfPacketsToSend),
                          MakeUintegerChecker<uint64_t>())
            .AddTraceSource("Tx",
                            "A new packet is sent",
                            MakeTraceSourceAccessor(&SenderApplication::m_txTrace),
                            "ns3::Packet::TracedCallback")
            .AddTraceSource("TxWithSeqTsSize",
                            "A new packet is created with SeqTsSizeHeader",
                            MakeTraceSourceAccessor(&SenderApplication::m_txTraceWithSeqTsSize),
                            "ns3::PacketSink::SeqTsSizeCallback");
    return tid;
}

SenderApplication::SenderApplication() : m_totBytes(0),
                                         m_unsentPacket(nullptr),
                                         availableInterfaces(),
                                         strategyType(LINEAR)
{
    this->availableInterfaces = std::make_shared<std::vector<std::shared_ptr<ISPInterface>>>(0);
    NS_LOG_FUNCTION(this);
}

SenderApplication::~SenderApplication()
{
    NS_LOG_FUNCTION(this);
}

void SenderApplication::SetMaxBytes(uint64_t maxBytes)
{
    NS_LOG_FUNCTION(this << maxBytes);
    amountOfPacketsToSend = maxBytes;
}

void SenderApplication::DoDispose()
{
    NS_LOG_FUNCTION(this);

    for (const auto &interface : *availableInterfaces)
    {
        interface->socketInfo->Close();
    }

    m_unsentPacket = nullptr;
    // chain up
    Application::DoDispose();
}

std::shared_ptr<std::vector<std::shared_ptr<ISPInterface>>> SenderApplication::getInterfaces()
{
    return this->availableInterfaces;
}

bool SenderApplication::HasAlreadyInitSocket(Address &from)
{
    return std::any_of(this->availableInterfaces->begin(), availableInterfaces->end(), [from](std::shared_ptr<ISPInterface> interface)
                       { return interface->outgoingAddress == from; });
}

// Application Methods
void SenderApplication::StartApplication() // Called at time specified by Start
{
    NS_LOG_FUNCTION(this);

    Ptr<Node> node = this->GetNode();
    Ptr<Ipv4> ipv4Node = node->GetObject<ns3::Ipv4>();

    // Saltiamo la prima interfaccia di rete perchè è quella di loopback

    uint32_t startingInterface = 1;
    for (uint32_t i = startingInterface; i < ipv4Node->GetNInterfaces(); ++i)
    {
        for (uint32_t j = 0; j < ipv4Node->GetNAddresses(i); ++j)
        {
            Ptr<NetDevice> device = ipv4Node->GetNetDevice(i);
            Ipv4Address addr = ipv4Node->GetAddress(i, j).GetLocal();
            std::cout << "Send Interface " << i << " IP Address " << j << ": " << addr << std::endl;
            Address address = InetSocketAddress(addr, Utils::ConnectionPort);
            Address destinationAddress = this->addresses->at(i - 1);
            uint32_t cost = this->costs->at(i - 1);
            PointerValue errorModelValue;
            device->GetAttribute("ReceiveErrorModel", errorModelValue);
            Ptr<RateErrorModel> errorModel = errorModelValue.Get<RateErrorModel>();
            InitSocket(device, address, destinationAddress, *errorModel, cost); // 0x555555699b80
        }
    }

    switch (this->strategyType)
    {
    case LINEAR:
        this->strategy = std::make_unique<LinearStrategy>(this->application, this->availableInterfaces);
        break;
    case RANDOM:
        this->strategy = std::make_unique<RandomStrategy>(this->application, this->availableInterfaces);
        break;
    case ROUND_ROBIN:
        this->strategy = std::make_unique<RoundRobinStrategy>(this->application, this->availableInterfaces);
        break;
    case REACTIVE:
        this->strategy = std::make_unique<ReactiveStrategy>(this->application, this->availableInterfaces);
        break;
    default:
        break;
    }

    if (this->strategy == nullptr)
    {
        std::cout << "Non è stato possibile determinare la strategia" << "\n";
        return;
    }

    std::for_each(this->strategy->applications->begin(), this->strategy->applications->end(), [](const std::shared_ptr<SDWanApplication> &application)
                  { application->OnApplicationStart(); });

    for (std::shared_ptr<ISPInterface> &e : *(this->strategy->availableInterfaces))
    {
        InitInterfaceEventLoop(e);
    }

    this->ComputeStrategyAndContinue();
}

void SenderApplication::ComputeStrategyAndContinue()
{

    std::for_each(this->strategy->applications->begin(), this->strategy->applications->end(), [](const std::shared_ptr<SDWanApplication> &application)
                  { application->Update(); });

    this->strategy->Compute();

    if (!this->strategy->getAllDataHasBeenSent())
    {
        Time tNext(Seconds(1));
        Simulator::Schedule(tNext, &SenderApplication::ComputeStrategyAndContinue, this);
        return;
    }
}

// Todo rinominare in initdevice
void SenderApplication::InitSocket(
    Ptr<NetDevice> device,
    Address &from,
    Address &destinationAddress,
    RateErrorModel &errorModel,
    uint32_t cost)
{

    // Create the socket if not already
    if (!this->HasAlreadyInitSocket(from))
    {
        Ptr<Socket> maybeSocket = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());

        uint32_t interfaceId = getNextInterfaceId();
        std::string interfaceName = "Interfaccia " + std::to_string(interfaceId);
        ISPInterface interface(interfaceId, interfaceName, device, from, maybeSocket, destinationAddress, errorModel, cost);
        // matchingInterface = std::make_shared<ISPInterface>(interface);
        this->availableInterfaces->emplace_back(std::make_shared<ISPInterface>(interface));

        int ret = -1;

        // Fatal error if socket type is not NS3_SOCK_STREAM or NS3_SOCK_SEQPACKET
        if (maybeSocket->GetSocketType() != Socket::NS3_SOCK_STREAM &&
            maybeSocket->GetSocketType() != Socket::NS3_SOCK_SEQPACKET)
        {
            NS_FATAL_ERROR("Using BulkSend with an incompatible socket type. "
                           "BulkSend requires SOCK_STREAM or SOCK_SEQPACKET. "
                           "In other words, use TCP instead of UDP.");
        }

        NS_ABORT_MSG_IF(destinationAddress.IsInvalid(), "Invalid destination address");

        if (!from.IsInvalid())
        {
            NS_ABORT_MSG_IF((Inet6SocketAddress::IsMatchingType(destinationAddress) &&
                             InetSocketAddress::IsMatchingType(from)) ||
                                (InetSocketAddress::IsMatchingType(destinationAddress) &&
                                 Inet6SocketAddress::IsMatchingType(from)),
                            "Incompatible peer and local address IP version");
            ret = maybeSocket->Bind(from);
        }
        else
        {
            if (Inet6SocketAddress::IsMatchingType(destinationAddress))
            {
                ret = maybeSocket->Bind6();
            }
            else if (InetSocketAddress::IsMatchingType(destinationAddress))
            {
                ret = maybeSocket->Bind();
            }
        }

        if (ret == -1)
        {
            NS_FATAL_ERROR("Failed to bind socket");
        }

        if (InetSocketAddress::IsMatchingType(destinationAddress))
        {
            maybeSocket->SetIpTos(m_tos); // Affects only IPv4 sockets.
        }
        maybeSocket->Connect(destinationAddress);
        maybeSocket->ShutdownRecv();
        maybeSocket->SetConnectCallback(MakeCallback(&SenderApplication::ConnectionSucceeded, this),
                                        MakeCallback(&SenderApplication::ConnectionFailed, this));
    }
    else
    {
        auto currentInterface = std::find_if(this->availableInterfaces->begin(), this->availableInterfaces->end(), [from](std::shared_ptr<ISPInterface> interface)
                                             { return interface->outgoingAddress == from; });
    }
}

void SenderApplication::InitInterfaceEventLoop(std::shared_ptr<ISPInterface> interface)
{
    Simulator::Schedule(Seconds(0), &SenderApplication::SendPacket, this, interface);
}

void SenderApplication::SendPacket(std::shared_ptr<ISPInterface> interface)
{
    if (this->strategy->getAllDataHasBeenSent())
    {
        return;
    }

    this->SendData(interface);
}

void SenderApplication::StopApplication() // Called at time specified by Stop
{
    NS_LOG_FUNCTION(this);

    std::vector<std::shared_ptr<ISPInterface>>::iterator interface;

    if (interface != this->availableInterfaces->end())
    {

        for (interface = this->availableInterfaces->begin(); interface != this->availableInterfaces->end(); interface++)
        {
            Ptr<Socket> socket = (*interface)->socketInfo;

            socket->Close();
            (*interface)->connected = false;
        }
    }
    else
    {
        NS_LOG_WARN("SenderApplication found null socket to close in StopApplication");
    }
}

// Private helpers

void SenderApplication::SendData(std::shared_ptr<ISPInterface> interface)
{
    auto m_socket = interface->socketInfo;
    auto m_connected = interface->connected;
    NS_LOG_FUNCTION(this);

    NS_LOG_LOGIC("sending packet at " << Simulator::Now());

    ns3::DataRate dataRate = interface->getDataRate();
    Time tNext(Seconds((Utils::PacketSizeBit) / static_cast<double>(dataRate.GetBitRate())));

    if (!interface->getHasAnyAvailablePackage())
    {
        Simulator::Schedule(tNext, &SenderApplication::SendPacket, this, interface);
        return;
    }

    if (m_socket->GetTxAvailable() < Utils::PacketSizeByte)
    {
        Simulator::Schedule(tNext, &SenderApplication::SendPacket, this, interface);
        return;
    }

    auto packetInfo = interface->getNextPacket().second;

    Ptr<Packet> packet = Create<Packet>(Utils::PacketSizeByte);

    FlowIdTag flowTag;
    flowTag.SetFlowId(packetInfo.originatedFrom);
    packet->AddByteTag(flowTag);

    uint32_t toSend = packet->GetSize();

    if (interface->errorModel.IsCorrupt(packet))
    {
        // std::cout << "Pacchetto corrotto: non verrà inviato" << std::endl;
        interface->corruptPackages++;
    }
    else
    {

        int actual = m_socket->Send(packet);
        if ((unsigned)actual == Utils::PacketSizeByte)
        {
            interface->correctPackages++;
            m_totBytes += actual;
            m_txTrace(packet);
            m_unsentPacket = nullptr;
        }
        else
        {
            std::cout << "Actual: " << actual << std::endl;
            std::cout << "toSend: " << toSend << std::endl;
            std::cout << "Errore: " << m_socket->GetErrno() << std::endl;
            std::cout << "Byte mandabili : " << m_socket->GetTxAvailable() << std::endl;
            std::cout << "Bytes totali: " << m_totBytes << std::endl;
            NS_FATAL_ERROR("Unexpected return value from m_socket->Send ()");
        }
    }
    Simulator::Schedule(tNext, &SenderApplication::SendPacket, this, interface);
}

std::vector<std::shared_ptr<ISPInterface>>::iterator SenderApplication::GetMatchingInterface(Ptr<Socket> socket)
{
    return std::find_if(this->availableInterfaces->begin(), this->availableInterfaces->end(), [socket](std::shared_ptr<ISPInterface> interface)
                        { return interface->socketInfo == socket; });
}

void SenderApplication::ConnectionSucceeded(Ptr<Socket> socket)
{
    std::vector<std::shared_ptr<ISPInterface>>::iterator interface = this->GetMatchingInterface(socket);
    NS_LOG_FUNCTION(this << socket);
    NS_LOG_LOGIC("SenderApplication Connection succeeded");
    (*interface)->connected = true;

    Address from;
    Address to;
    socket->GetSockName(from);
    socket->GetPeerName(to);
}

void SenderApplication::ConnectionFailed(Ptr<Socket> socket)
{
    NS_LOG_FUNCTION(this << socket);
    NS_LOG_LOGIC("SenderApplication, Connection Failed");
}
