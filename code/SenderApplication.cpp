/*
 * Copyright (c) 2010 Georgia Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: George F. Riley <riley@ece.gatech.edu>
 */

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

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SenderApplication");

NS_OBJECT_ENSURE_REGISTERED(SenderApplication);

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
                                         availableInterfaces()
{
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

    for (const auto &interface : availableInterfaces)
    {
        interface.socketInfo->Close();
    }

    m_unsentPacket = nullptr;
    // chain up
    Application::DoDispose();
}

bool SenderApplication::HasAlreadyInitSocket(Address &from)
{
    return std::any_of(this->availableInterfaces.begin(), availableInterfaces.end(), [from](const ISPInterface &interface)
                       { return interface.outgoingAddress == from; });
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
            Ipv4Address addr = ipv4Node->GetAddress(i, j).GetLocal();
            std::cout << "Send Interface " << i << " IP Address " << j << ": " << addr << std::endl;
            Address address = InetSocketAddress(addr, Utils::ConnectionPort);
            Address destinationAddress = this->addresses->at(i - 1);
            Ptr<NetDevice> device = node->GetDevice(j);
            PointerValue errorModelValue;
            device->GetAttribute("ReceiveErrorModel", errorModelValue);
            Ptr<RateErrorModel> errorModel = errorModelValue.Get<RateErrorModel>();
            InitSocket(address, destinationAddress, *errorModel);
        }
    }

    ns3::Simulator::Schedule(Seconds(2), &SenderApplication::SendPacket, this);
}

void SenderApplication::InitSocket(
    Address &from,
    Address &destinationAddress,
    RateErrorModel &errorModel)
{

    std::unique_ptr<ISPInterface> matchingInterface;

    // Create the socket if not already
    if (!this->HasAlreadyInitSocket(from))
    {
        Ptr<Socket> maybeSocket = Socket::CreateSocket(GetNode(), TcpSocketFactory::GetTypeId());
        ISPInterface interface(from, maybeSocket, destinationAddress, errorModel);
        matchingInterface = std::make_unique<ISPInterface>(interface);
        this->availableInterfaces.push_back(interface);
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
        auto currentInterface = std::find_if(this->availableInterfaces.begin(), this->availableInterfaces.end(), [from](const ISPInterface &interface)
                                             { return interface.outgoingAddress == from; });

        matchingInterface = std::make_unique<ISPInterface>(*currentInterface);
    }

    if (matchingInterface->connected)
    {

        matchingInterface->socketInfo->GetSockName(from);
    }
}

void SenderApplication::SendPacket()
{
    this->SendData(this->availableInterfaces.at(0));
}

void SenderApplication::StopApplication() // Called at time specified by Stop
{
    NS_LOG_FUNCTION(this);

    std::vector<ISPInterface>::iterator interface;

    if (interface != this->availableInterfaces.end())
    {

        for (interface = this->availableInterfaces.begin(); interface != this->availableInterfaces.end(); interface++)
        {
            Ptr<Socket> socket = interface->socketInfo;

            socket->Close();
            interface->connected = false;
        }
    }
    else
    {
        NS_LOG_WARN("SenderApplication found null socket to close in StopApplication");
    }
}

// Private helpers

void SenderApplication::SendData(ISPInterface &interface)
{
    auto m_socket = interface.socketInfo;
    auto m_connected = interface.connected;
    NS_LOG_FUNCTION(this);

    // Time to send more

    // uint64_t to allow the comparison later.
    // the result is in a uint32_t range anyway, because
    // m_sendSize is uint32_t.
    uint64_t toSend = amountOfPacketsToSend;
    // Make sure we don't send too many
    if (amountOfPacketsToSend > 0)
    {
        toSend = std::min(toSend, amountOfPacketsToSend - m_totBytes);
    }

    NS_LOG_LOGIC("sending packet at " << Simulator::Now());

    Ptr<Packet> packet;

    if (interface.errorModel.IsCorrupt(packet))
    {
        std::cout << "Pacchetto corrotto: non verrà inviato" << std::endl;
        interface.corruptPackages++;
        return;
    }

    if (m_unsentPacket)
    {
        packet = m_unsentPacket;
        toSend = packet->GetSize();
    }
    else
    {
        packet = Create<Packet>(toSend);
    }

    int actual = m_socket->Send(packet);
    if ((unsigned)actual == toSend)
    {
        interface.correctPackages++;
        m_totBytes += actual;
        m_txTrace(packet);
        m_unsentPacket = nullptr;
    }
    else if (actual == -1)
    {
        // We exit this loop when actual < toSend as the send side
        // buffer is full. The "DataSent" callback will pop when
        // some buffer space has freed up.
        NS_LOG_DEBUG("Unable to send packet; caching for later attempt");
        m_unsentPacket = packet;
    }
    else if (actual > 0 && (unsigned)actual < toSend)
    {
        // A Linux socket (non-blocking, such as in DCE) may return
        // a quantity less than the packet size.  Split the packet
        // into two, trace the sent packet, save the unsent packet
        NS_LOG_DEBUG("Packet size: " << packet->GetSize() << "; sent: " << actual
                                     << "; fragment saved: " << toSend - (unsigned)actual);
        Ptr<Packet> sent = packet->CreateFragment(0, actual);
        Ptr<Packet> unsent = packet->CreateFragment(actual, (toSend - (unsigned)actual));
        m_totBytes += actual;
        m_txTrace(sent);
        m_unsentPacket = unsent;
    }
    else
    {
        NS_FATAL_ERROR("Unexpected return value from m_socket->Send ()");
    }

    // Check if time to close (all sent)
    if (m_totBytes == amountOfPacketsToSend && m_connected)
    {
        m_socket->Close();
        m_connected = false;
    }
}

std::vector<ISPInterface>::iterator SenderApplication::GetMatchingInterface(Ptr<Socket> socket)
{
    return std::find_if(this->availableInterfaces.begin(), this->availableInterfaces.end(), [socket](const ISPInterface &interface)
                        { return interface.socketInfo == socket; });
}

void SenderApplication::ConnectionSucceeded(Ptr<Socket> socket)
{
    std::vector<ISPInterface>::iterator interface = this->GetMatchingInterface(socket);
    NS_LOG_FUNCTION(this << socket);
    NS_LOG_LOGIC("SenderApplication Connection succeeded");
    interface->connected = true;

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
