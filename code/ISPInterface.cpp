#include "ISPInterface.h"

ISPInterface::ISPInterface(const ns3::Address &outgoingAddress,
                           ns3::Ptr<ns3::Socket> socketInfo,
                           const ns3::Address &destinationAddress,
                           ns3::RateErrorModel &errorModel) : outgoingAddress(outgoingAddress),
                                                                    socketInfo(socketInfo),
                                                                    destinationAddress(destinationAddress),
                                                                    errorModel(errorModel),
                                                                    corruptPackages(0),
                                                                    correctPackages(0),
                                                                    connected(false)
{
}