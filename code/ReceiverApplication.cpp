
#include "ReceiverApplication.h"
#include "Utils.h"
#include "ns3/address-utils.h"
#include "ns3/address.h"
#include "ns3/boolean.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/ipv4-packet-info-tag.h"
#include "ns3/ipv6-packet-info-tag.h"
#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/socket.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/udp-socket.h"
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
#include "ns3/core-module.h"
#include "ns3/network-module.h"

namespace ns3
{

    NS_LOG_COMPONENT_DEFINE("ReceiverApplication");

    NS_OBJECT_ENSURE_REGISTERED(ReceiverApplication);

    TypeId
    ReceiverApplication::GetTypeId()
    {
        static TypeId tid =
            TypeId("ReceiverApplication")
                .SetParent<Application>()
                .SetGroupName("Applications")
                .AddConstructor<ReceiverApplication>()
                .AddAttribute("EnableSeqTsSizeHeader",
                              "Enable optional header tracing of SeqTsSizeHeader",
                              BooleanValue(false),
                              MakeBooleanAccessor(&ReceiverApplication::m_enableSeqTsSizeHeader),
                              MakeBooleanChecker())
                .AddTraceSource("Rx",
                                "A packet has been received",
                                MakeTraceSourceAccessor(&ReceiverApplication::m_rxTrace),
                                "ns3::Packet::AddressTracedCallback")
                .AddTraceSource("RxWithAddresses",
                                "A packet has been received",
                                MakeTraceSourceAccessor(&ReceiverApplication::m_rxTraceWithAddresses),
                                "ns3::Packet::TwoAddressTracedCallback")
                .AddTraceSource("RxWithSeqTsSize",
                                "A packet with SeqTsSize header has been received",
                                MakeTraceSourceAccessor(&ReceiverApplication::m_rxTraceWithSeqTsSize),
                                "ns3::ReceiverApplication::SeqTsSizeCallback");
        return tid;
    }

    ReceiverApplication::ReceiverApplication()
        : listeningSocketInfo()
    {
        NS_LOG_FUNCTION(this);
        m_totalRx = 0;
    }

    ReceiverApplication::~ReceiverApplication()
    {
        NS_LOG_FUNCTION(this);
    }

    uint64_t
    ReceiverApplication::GetTotalRx() const
    {
        NS_LOG_FUNCTION(this);
        return m_totalRx;
    }

    std::list<Ptr<Socket>>
    ReceiverApplication::GetAcceptedSockets() const
    {
        NS_LOG_FUNCTION(this);
        return m_socketList;
    }

    void
    ReceiverApplication::DoDispose()
    {
        NS_LOG_FUNCTION(this);

        std::map<Address, Ptr<Socket>>::iterator keyValuePair;

        for (keyValuePair = this->listeningSocketInfo.begin(); keyValuePair != this->listeningSocketInfo.end(); keyValuePair++)
        {
            Ptr<Socket> socket = keyValuePair->second;

            socket = nullptr;
        }

        this->listeningSocketInfo.clear();
        m_socketList.clear();

        // chain up
        Application::DoDispose();
    }

    // Application Methods
    void
    ReceiverApplication::StartApplication() // Called at time specified by Start
    {
        NS_LOG_FUNCTION(this);

        Ptr<Node> node = this->GetNode();
        Ptr<ns3::Ipv4> ipv4Node = node->GetObject<ns3::Ipv4>();

        // Saltiamo la prima interfaccia di rete perchè è quella di loopback

        uint32_t startingInterface = 1;
        for (uint32_t i = startingInterface; i < ipv4Node->GetNInterfaces(); ++i)
        {
            for (uint32_t j = 0; j < ipv4Node->GetNAddresses(i); ++j)
            {
                Ipv4Address addr = ipv4Node->GetAddress(i, j).GetLocal();
                std::cout << "Receive Interface " << i << " IP Address " << j << ": " << addr << std::endl;
                Address address = InetSocketAddress(addr, Utils::ConnectionPort);
                InitReceivingSocket(address);
            }
        }
    }

    void ReceiverApplication::InitReceivingSocket(ns3::Address &interfaceAddress)
    {
        Ptr<Socket> m_socket = nullptr;
        if ((this->listeningSocketInfo.find(interfaceAddress) == this->listeningSocketInfo.end()))
        {

            m_socket = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
            this->listeningSocketInfo.insert_or_assign(interfaceAddress, m_socket);
            NS_ABORT_MSG_IF(interfaceAddress.IsInvalid(), "'Local' attribute not properly set");
            if (m_socket->Bind(interfaceAddress) == -1)
            {
                NS_FATAL_ERROR("Failed to bind socket");
            }
            m_socket->Listen();
            m_socket->ShutdownSend();
            if (addressUtils::IsMulticast(interfaceAddress))
            {
                Ptr<UdpSocket> udpSocket = DynamicCast<UdpSocket>(m_socket);
                if (udpSocket)
                {
                    // equivalent to setsockopt (MCAST_JOIN_GROUP)
                    udpSocket->MulticastJoinGroup(0, interfaceAddress);
                }
                else
                {
                    NS_FATAL_ERROR("Error: joining multicast on a non-UDP socket");
                }
            }
        }
        else
        {
            m_socket = this->listeningSocketInfo.find(interfaceAddress)->second;
        }

        if (InetSocketAddress::IsMatchingType(interfaceAddress))
        {
            m_localPort = InetSocketAddress::ConvertFrom(interfaceAddress).GetPort();
        }
        else if (Inet6SocketAddress::IsMatchingType(interfaceAddress))
        {
            m_localPort = Inet6SocketAddress::ConvertFrom(interfaceAddress).GetPort();
        }
        else
        {
            m_localPort = 0;
        }
        m_socket->SetRecvCallback(MakeCallback(&ReceiverApplication::HandleRead, this));
        m_socket->SetRecvPktInfo(true);
        m_socket->SetAcceptCallback(MakeNullCallback<bool, Ptr<Socket>, const Address &>(),
                                    MakeCallback(&ReceiverApplication::HandleAccept, this));
        m_socket->SetCloseCallbacks(MakeCallback(&ReceiverApplication::HandlePeerClose, this),
                                    MakeCallback(&ReceiverApplication::HandlePeerError, this));
    }

    void
    ReceiverApplication::StopApplication() // Called at time specified by Stop
    {
        NS_LOG_FUNCTION(this);
        while (!m_socketList.empty()) // these are accepted sockets, close them
        {
            Ptr<Socket> acceptedSocket = m_socketList.front();
            m_socketList.pop_front();
            acceptedSocket->Close();
        }

        std::map<Address, Ptr<Socket>>::iterator keyValuePair;

        for (keyValuePair = this->listeningSocketInfo.begin(); keyValuePair != this->listeningSocketInfo.end(); keyValuePair++)
        {
            Ptr<Socket> socket = keyValuePair->second;
            if (socket)
            {

                socket->Close();
                socket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket>>());
            }
        }
    }

    void
    ReceiverApplication::HandleRead(Ptr<Socket> socket)
    {
        NS_LOG_FUNCTION(this << socket);
        Ptr<Packet> packet;
        Address from;
        Address localAddress;
        while ((packet = socket->RecvFrom(from)))
        {
            if (packet->GetSize() == 0)
            { // EOF
                break;
            }
            m_totalRx += packet->GetSize();

            // Recupero del TimestampTag dal pacchetto
            TimestampTag timestamp;
            // Should never not be found since the sender is adding it, but
            // you never know.
            if (packet->FindFirstMatchingByteTag(timestamp))
            {
                Time tx = timestamp.GetTimestamp();

                std::cout << "Timestamp ricevuto: "
                          << timestamp.GetTimestamp().GetNanoSeconds()
                          << " ns" << std::endl;
            }

            if (InetSocketAddress::IsMatchingType(from))
            {
                NS_LOG_INFO("At time " << Simulator::Now().As(Time::S) << " packet sink received "
                                       << packet->GetSize() << " bytes from "
                                       << InetSocketAddress::ConvertFrom(from).GetIpv4() << " port "
                                       << InetSocketAddress::ConvertFrom(from).GetPort() << " total Rx "
                                       << m_totalRx << " bytes");
            }
            else if (Inet6SocketAddress::IsMatchingType(from))
            {
                NS_LOG_INFO("At time " << Simulator::Now().As(Time::S) << " packet sink received "
                                       << packet->GetSize() << " bytes from "
                                       << Inet6SocketAddress::ConvertFrom(from).GetIpv6() << " port "
                                       << Inet6SocketAddress::ConvertFrom(from).GetPort()
                                       << " total Rx " << m_totalRx << " bytes");
            }

            if (!m_rxTrace.IsEmpty() || !m_rxTraceWithAddresses.IsEmpty() ||
                (!m_rxTraceWithSeqTsSize.IsEmpty() && m_enableSeqTsSizeHeader))
            {
                Ipv4PacketInfoTag interfaceInfo;
                Ipv6PacketInfoTag interface6Info;
                if (packet->RemovePacketTag(interfaceInfo))
                {
                    localAddress = InetSocketAddress(interfaceInfo.GetAddress(), m_localPort);
                }
                else if (packet->RemovePacketTag(interface6Info))
                {
                    localAddress = Inet6SocketAddress(interface6Info.GetAddress(), m_localPort);
                }
                else
                {
                    socket->GetSockName(localAddress);
                }
                m_rxTrace(packet, from);
                m_rxTraceWithAddresses(packet, from, localAddress);

                if (!m_rxTraceWithSeqTsSize.IsEmpty() && m_enableSeqTsSizeHeader)
                {
                    PacketReceived(packet, from, localAddress);
                }
            }

            // Recupero del TimestampTag dal pacchetto
            auto iterator = packet->GetPacketTagIterator();
            while (iterator.HasNext())
            {
                std::cout << "Tag" << "\n";
            }
        }
    }

    void
    ReceiverApplication::PacketReceived(const Ptr<Packet> &p, const Address &from, const Address &localAddress)
    {

        SeqTsSizeHeader header;
        Ptr<Packet> buffer;

        auto itBuffer = m_buffer.find(from);
        if (itBuffer == m_buffer.end())
        {
            itBuffer = m_buffer.insert(std::make_pair(from, Create<Packet>(0))).first;
        }

        buffer = itBuffer->second;
        buffer->AddAtEnd(p);
        buffer->PeekHeader(header);

        NS_ABORT_IF(header.GetSize() == 0);

        while (buffer->GetSize() >= header.GetSize())
        {
            NS_LOG_DEBUG("Removing packet of size " << header.GetSize() << " from buffer of size "
                                                    << buffer->GetSize());
            Ptr<Packet> complete = buffer->CreateFragment(0, static_cast<uint32_t>(header.GetSize()));
            buffer->RemoveAtStart(static_cast<uint32_t>(header.GetSize()));

            complete->RemoveHeader(header);

            m_rxTraceWithSeqTsSize(complete, from, localAddress, header);

            if (buffer->GetSize() > header.GetSerializedSize())
            {
                buffer->PeekHeader(header);
            }
            else
            {
                break;
            }
        }
    }

    void
    ReceiverApplication::HandlePeerClose(Ptr<Socket> socket)
    {
        NS_LOG_FUNCTION(this << socket);
    }

    void
    ReceiverApplication::HandlePeerError(Ptr<Socket> socket)
    {
        NS_LOG_FUNCTION(this << socket);
    }

    void
    ReceiverApplication::HandleAccept(Ptr<Socket> s, const Address &from)
    {
        NS_LOG_FUNCTION(this << s << from);
        s->SetRecvCallback(MakeCallback(&ReceiverApplication::HandleRead, this));
        m_socketList.push_back(s);
    }

} // Namespace ns3
