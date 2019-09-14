#ifndef NETWORK_CANUDPRECEIVER_H
#define NETWORK_CANUDPRECEIVER_H


#include <cstdint>
#include <gsl/gsl>
#include <QObject>

#include "tincan/canrawframe.h"
#include "network/udpasyncreceiver.h"


namespace network {


class Can_udp_receiver final : public QObject, public udp::Async_receiver
{
  Q_OBJECT

public:
  Can_udp_receiver() = default;
  Can_udp_receiver(const Can_udp_receiver&) = delete;
  Can_udp_receiver(Can_udp_receiver&&) = delete;
  Can_udp_receiver& operator=(const Can_udp_receiver&) = delete;
  Can_udp_receiver& operator=(Can_udp_receiver&&) = delete;

  void handle_receive(gsl::span<std::uint8_t> buffer) override;

signals:
  void received_frame(std::uint64_t, tin::Can_raw_frame);
};


}  // namespace network


#endif  // NETWORK_CANUDPRECEIVER_H
