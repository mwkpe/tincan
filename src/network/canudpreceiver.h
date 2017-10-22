#ifndef CAN_UDPRECEIVER_H
#define CAN_UDPRECEIVER_H


#include <cstdint>
#include <gsl/gsl>
#include <QObject>

#include "canrawframe.h"
#include "udpasyncreceiver.h"


namespace can
{


class Udp_receiver final : public QObject, public net::udp::Async_receiver
{
  Q_OBJECT

public:
  Udp_receiver() = default;
  Udp_receiver(const Udp_receiver&) = delete;
  Udp_receiver(Udp_receiver&&) = delete;
  Udp_receiver& operator=(const Udp_receiver&) = delete;
  Udp_receiver& operator=(Udp_receiver&&) = delete;

  void handle_receive(gsl::span<std::uint8_t> buffer) override;

signals:
  void received_frame(std::uint64_t, can::Raw_frame);
};


}  // namespace can


#endif  // CAN_UDPRECEIVER_H
