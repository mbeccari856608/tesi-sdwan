
#ifndef SENDER_APPLICATION_H
#define SENDER_APPLICATION_H

#include "ns3/seq-ts-size-header.h"

#include "ns3/address.h"
#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/error-model.h"
#include "ns3/traced-callback.h"
#include "ISPInterface.h"
#include "SDWanApplication.h"
#include <vector>
#include <map>


using namespace ns3;

// todo rename to CPEApplication.
class SenderApplication : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId();

  SenderApplication();

  ~SenderApplication() override;

  /**
   * \brief Set the upper bound for the total number of bytes to send.
   *
   * Once this bound is reached, no more application bytes are sent. If the
   * application is stopped during the simulation and restarted, the
   * total number of bytes sent is not reset; however, the maxBytes
   * bound is still effective and the application will continue sending
   * up to maxBytes. The value zero for maxBytes means that
   * there is no upper bound; i.e. data is sent until the application
   * or simulation is stopped.
   *
   * \param maxBytes the upper bound of bytes to send
   */
  void SetMaxBytes(uint64_t maxBytes);

  /**
   * \brief Get the socket this application is attached to.
   * \return pointer to associated socket
   */
  Ptr<Socket> GetSocket() const;

  /**
   * \brief Pointer to array containing all available destination addresses.
   */
  std::unique_ptr<std::vector<Address>> addresses;

    /**
   * \brief Pointer to array containing all the applications on the CPE
   */
  std::unique_ptr<std::vector<SDWanApplication>> application;

protected:
  void DoDispose() override;

private:
  void StartApplication() override;
  void StopApplication() override;

  void SendData(ISPInterface &interface);

  std::vector<ISPInterface> availableInterfaces; //!< List of the interfaces that can be used to send data.
  uint8_t m_tos;                                 //!< The packets Type of Service
  uint64_t amountOfPacketsToSend;                //!< Limit total number of bytes sent
  uint64_t m_totBytes;                           //!< Total bytes sent so far
  TypeId m_tid;                                  //!< The type of protocol to use.
  uint32_t m_seq{0};                             //!< Sequence
  Ptr<Packet> m_unsentPacket;                    //!< Variable to cache unsent packet

  /// Traced Callback: sent packets
  TracedCallback<Ptr<const Packet>> m_txTrace;

  /// Callback for tracing the packet Tx events, includes source, destination,  the packet sent,
  /// and header
  TracedCallback<Ptr<const Packet>, const Address &, const Address &, const SeqTsSizeHeader &>
      m_txTraceWithSeqTsSize;

private:
  /**
   * \brief Connection Succeeded (called by Socket through a callback)
   * \param socket the connected socket
   */
  void ConnectionSucceeded(Ptr<Socket> socket);
  /**
   * \brief Connection Failed (called by Socket through a callback)
   * \param socket the connected socket
   */
  void ConnectionFailed(Ptr<Socket> socket);

  void InitSocket(Address &from, Address &destinationAddress, RateErrorModel &errorModel);

  bool HasAlreadyInitSocket(Address &from);

  std::vector<ISPInterface>::iterator GetMatchingInterface(Ptr<Socket> socket);

  void SendPacket();
};

#endif /* SENDER_APPLICATION_H */
