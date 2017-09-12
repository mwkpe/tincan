#ifndef TIN_CAN_RECEIVER_H
#define TIN_CAN_RECEIVER_H


#include <cstdint>
#include <string>
#include <fstream>

class QNetworkDatagram;

#include "canframe.h"
#include "udpreceiver.h"


namespace tin
{


class CanReceiver : public UdpReceiver
{
  Q_OBJECT

public:
  CanReceiver() = default;
  CanReceiver(const CanReceiver&) = delete;
  CanReceiver(CanReceiver&&) = delete;
  CanReceiver& operator=(const CanReceiver&) = delete;
  CanReceiver& operator=(CanReceiver&&) = delete;

  void handle_received_datagram(const QNetworkDatagram* datagram) override;

signals:
  void received_frame(std::uint64_t, CanFrame);
};


}  // namespace tin


#endif  // TIN_CAN_RECEIVER_H
