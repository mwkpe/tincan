#ifndef CAN_RECEIVER_H
#define CAN_RECEIVER_H


#include <cstdint>
#include <gsl/gsl>
#include <QObject>
#include <QMetaType>

#include "canrawframe.h"
#include "udpasyncreceiver.h"


namespace can
{


class Receiver final : public QObject, public net::udp::Async_receiver
{
  Q_OBJECT

public:
  Receiver() = default;
  Receiver(const Receiver&) = delete;
  Receiver(Receiver&&) = delete;
  Receiver& operator=(const Receiver&) = delete;
  Receiver& operator=(Receiver&&) = delete;

  void handle_receive(gsl::span<std::uint8_t> buffer) override;

signals:
  void received_frame(std::uint64_t, can::Raw_frame);
  void received_frame_id(std::uint32_t);
};


}  // namespace can


#endif  // CAN_RECEIVER_H
