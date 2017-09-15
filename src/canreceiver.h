#ifndef CAN_RECEIVER_H
#define CAN_RECEIVER_H


#include <cstdint>

class QNetworkDatagram;

#include "canframe.h"
#include "udpreceiver.h"


namespace can
{


class receiver : public udp::receiver
{
  Q_OBJECT

public:
  receiver() = default;
  receiver(const receiver&) = delete;
  receiver(receiver&&) = delete;
  receiver& operator=(const receiver&) = delete;
  receiver& operator=(receiver&&) = delete;

  void handle_received_datagram(const QNetworkDatagram* datagram) override;

signals:
  void received_frame(std::uint64_t, can::frame);
};


}  // namespace can


#endif  // CAN_RECEIVER_H
