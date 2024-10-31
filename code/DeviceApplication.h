#ifndef DEVICEAPPLICATION_H
#define DEVICEAPPLICATION_H

#include "ns3/application.h"
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"

/**
 * @brief Classe utilizzata per rappresentare l'applicazione installata
 * sulle CPE
 * 
 */
class DeviceApplication : public ns3::Application
{
public:
    DeviceApplication();
    virtual ~DeviceApplication();
    static ns3::TypeId GetTypeId();
    void Setup(ns3::Ptr<ns3::Socket> socket,
               ns3::Address address);

private:
    void StartApplication() override;
    void StopApplication() override;

    void SendPacket();

    ns3::Ptr<ns3::Socket> transimissionSocket;
    ns3::Address destinationAddress;
};

#endif // DEVICEAPPLICATION_H


