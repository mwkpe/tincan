#ifndef CAN_RECEIVER_H
#define CAN_RECEIVER_H


#include <cstdint>

#include "canrawframe.h"
#include "udpreceiver.h"

class QNetworkDatagram;


namespace can
{


class Receiver : public udp::Receiver
{
  Q_OBJECT

public:
  Receiver() = default;
  Receiver(const Receiver&) = delete;
  Receiver(Receiver&&) = delete;
  Receiver& operator=(const Receiver&) = delete;
  Receiver& operator=(Receiver&&) = delete;

  void handle_received_datagram(const QNetworkDatagram* datagram) override;

signals:
  void received_frame(std::uint64_t, can::Raw_frame);
  void received_frame_id(std::uint32_t);
};


}  // namespace can


#endif  // CAN_RECEIVER_H
