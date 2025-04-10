#ifndef APPLICATION_SENDER_HELPER_H
#define APPLICATION_SENDER_HELPER_H

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "SDWanApplication.h"
#include "StrategyTypes.h"

class ApplicationSenderHelper : public ns3::ApplicationHelper
{
  public:

    /**
     * Create an ApplicationSenderHelper to make it easier to work with SenderApplications.
     * 
     * @param destinations The possible destination addresses: the first address is for the first interface,
     *                  the second one is for the second interface and so on.
     * 
     */
    ApplicationSenderHelper(
      std::vector<ns3::Address>& destinations,
      std::vector<std::shared_ptr<SDWanApplication>>& applications,
      std::vector<uint32_t> &costs,
      StrategyTypes strategy);

    /**
     * Due to how ns3 works we may need to do some manual tweaking when adding the application
     * to the device.
     */
    ns3::Ptr<ns3::Application> DoInstall(ns3::Ptr<ns3::Node> node) override;

    private:
      std::vector<ns3::Address>& addresses;
      std::vector<std::shared_ptr<SDWanApplication>>& applications;
      std::vector<uint32_t>& costs;
      StrategyTypes strategy;
};

#endif /* APPLICATION_SENDER_HELPER_H */
