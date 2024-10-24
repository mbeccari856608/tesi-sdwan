

// Network topology
//
//        n0     n1
//        |      |
//       ----------        ----------
//       | Switch |        | Switch |
//       ----------        ----------
//        |      |
//        n2     n3
//
//
// - CBR/UDP flows from n0 to n1 and from n3 to n0
// - DropTail queues
// - Tracing of queues and packet receptions to file "openflow-switch.tr"
// - If order of adding nodes and netdevices is kept:
//      n0 = 00:00:00;00:00:01, n1 = 00:00:00:00:00:03, n3 = 00:00:00:00:00:07
//      and port number corresponds to node number, so port 0 is connected to n0, for example.

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/log.h"
#include "ns3/network-module.h"
#include "ns3/openflow-module.h"

#include <fstream>
#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("OpenFlowCsmaSwitchExample");

bool verbose = false;
bool use_drop = false;
ns3::Time timeout = ns3::Seconds(0);

void handler(NetDeviceContainer& switchDevices)
{
    auto amount = switchDevices.GetN();

    std::cout << "\n";
    std::cout << "------------------------------------" << "\n";
    std::cout << "Ci sono " << amount << " dispositivi" << "\n";
    for (size_t i = 0; i < amount; i++)
    {
        auto device = switchDevices.Get(i);
        PointerValue ptr;
        device->GetAttribute("TxQueue", ptr);
        Ptr<Queue<Packet>> txQueue = ptr.Get<Queue<Packet>>();
        Ptr<DropTailQueue<Packet>> dtq = txQueue->GetObject<DropTailQueue<Packet>>();
        NS_ASSERT (dtq);
        std::cout << "Il dispositivo " << i << " ha "  << dtq->GetNPackets() << " pacchetti in coda" << "\n";
    }
    std::cout << "------------------------------------" << "\n";


    Simulator::Schedule(MilliSeconds(500), &handler, switchDevices);


}

bool
SetVerbose(const std::string& value)
{
    verbose = true;
    return true;
}

bool
SetDrop(const std::string& value)
{
    use_drop = true;
    return true;
}

bool
SetTimeout(const std::string& value)
{
    try
    {
        timeout = ns3::Seconds(std::stof(value));
        return true;
    }
    catch (...)
    {
        return false;
    }
    return false;
}

void ParseCommandLineParameters(int argc, char* argv[]){
    //
    // Allow the user to override any of the defaults and the above Bind() at
    // run-time, via command-line arguments
    //
    CommandLine cmd(__FILE__);
    cmd.AddValue("v", "Verbose (turns on logging).", MakeCallback(&SetVerbose));
    cmd.AddValue("verbose", "Verbose (turns on logging).", MakeCallback(&SetVerbose));
    cmd.AddValue("d", "Use Drop Controller (Learning if not specified).", MakeCallback(&SetDrop));
    cmd.AddValue("drop",
                 "Use Drop Controller (Learning if not specified).",
                 MakeCallback(&SetDrop));
    cmd.AddValue("t",
                 "Learning Controller Timeout (has no effect if drop controller is specified).",
                 MakeCallback(&SetTimeout));
    cmd.AddValue("timeout",
                 "Learning Controller Timeout (has no effect if drop controller is specified).",
                 MakeCallback(&SetTimeout));

    cmd.Parse(argc, argv);
}

int main(int argc, char* argv[])
{
    ParseCommandLineParameters(argc, argv);

    if (verbose)
    {
        LogComponentEnable("OpenFlowCsmaSwitchExample", LOG_LEVEL_INFO);
        LogComponentEnable("OpenFlowInterface", LOG_LEVEL_INFO);
        LogComponentEnable("OpenFlowSwitchNetDevice", LOG_LEVEL_INFO);
    }

    //
    // Explicitly create the nodes required by the topology (shown above).
    //
    NS_LOG_INFO("Create nodes.");
    NodeContainer terminals;
    terminals.Create(4);

    NodeContainer csmaSwitch;
    csmaSwitch.Create(1);

    NS_LOG_INFO("Build Topology");
    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", DataRateValue(5000000));
    csma.SetChannelAttribute("Delay", TimeValue(Seconds(2)));

    // Create the csma links, from each terminal to the switch
    NetDeviceContainer terminalDevices;
    NetDeviceContainer switchDevices;
    for (int i = 0; i < 4; i++)
    {
        NetDeviceContainer link = csma.Install(NodeContainer(terminals.Get(i), csmaSwitch));
        terminalDevices.Add(link.Get(0));
        switchDevices.Add(link.Get(1));
    }

    // Create the switch netdevice, which will do the packet switching
    Ptr<Node> switchNode = csmaSwitch.Get(0);
    OpenFlowSwitchHelper swtch;

    if (use_drop)
    {
        Ptr<ns3::ofi::DropController> controller = CreateObject<ns3::ofi::DropController>();
        swtch.Install(switchNode, switchDevices, controller);
    }
    else
    {
        Ptr<ns3::ofi::LearningController> controller = CreateObject<ns3::ofi::LearningController>();
        if (!timeout.IsZero())
        {
            controller->SetAttribute("ExpirationTime", TimeValue(timeout));
        }
        
        swtch.Install(switchNode, switchDevices, controller);
    }

    auto test = switchNode->GetDevice(0);

    // Add internet stack to the terminals
    InternetStackHelper internet;
    internet.Install(terminals);

    // We've got the "hardware" in place.  Now we need to add IP addresses.
    NS_LOG_INFO("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    ipv4.Assign(terminalDevices);

    // Create an OnOff application to send UDP datagrams from n0 to n1.
    NS_LOG_INFO("Create Applications.");
    uint16_t port = 9; // Discard port (RFC 863)

    OnOffHelper onoff("ns3::TcpSocketFactory",
                      Address(InetSocketAddress(Ipv4Address("10.1.1.2"), port)));
    onoff.SetConstantRate(DataRate("100000Gb/s"));

    ApplicationContainer app = onoff.Install(terminals.Get(0));
    // Start the application
    app.Start(Seconds(0.0));
    app.Stop(Seconds(30));

    // Create an optional packet sink to receive these packets
    PacketSinkHelper sink("ns3::TcpSocketFactory",
                          Address(InetSocketAddress(Ipv4Address::GetAny(), port)));
    app = sink.Install(terminals.Get(1));
    app.Start(Seconds(0.0));

    //
    // Create a similar flow from n3 to n0, starting at time 1.1 seconds
    //
    onoff.SetAttribute("Remote", AddressValue(InetSocketAddress(Ipv4Address("10.1.1.1"), port)));
    app = onoff.Install(terminals.Get(3));
    app.Start(Seconds(1.1));
    app.Stop(Seconds(10.0));

    app = sink.Install(terminals.Get(0));
    app.Start(Seconds(0.0));

    NS_LOG_INFO("Configure Tracing.");

    //
    // Configure tracing of all enqueue, dequeue, and NetDevice receive events.
    // Trace output will be sent to the file "openflow-switch.tr"
    //
    AsciiTraceHelper ascii;
    csma.EnableAsciiAll(ascii.CreateFileStream("openflow-switch.tr"));

    //
    // Also configure some tcpdump traces; each interface will be traced.
    // The output files will be named:
    //     openflow-switch-<nodeId>-<interfaceId>.pcap
    // and can be read by the "tcpdump -r" command (use "-tt" option to
    // display timestamps correctly)
    //
    csma.EnablePcapAll("openflow-switch", false);

    //
    // Now, do the actual simulation.
    //
    NS_LOG_INFO("Run Simulation.");
    Simulator::Stop(Seconds(2));
    Simulator::Schedule(MilliSeconds(500), &handler, switchDevices);
    Simulator::Run();
    Simulator::Destroy();
    NS_LOG_INFO("Done.");

    return 0;
}


