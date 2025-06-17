#ifndef NETWORK_PCAN_USB_TRANSCEIVER_H
#define NETWORK_PCAN_USB_TRANSCEIVER_H


#include <cstdint>
#include <thread>
#include <vector>
#include <QObject>
#include "tincan/canrawframe.h"


namespace network {


enum class Pcan_baudrate
{
  Baud_100k,
  Baud_125k,
  Baud_250k,
  Baud_500k,
  Baud_800k,
  Baud_1m
};


struct Pcan_channel
{
  std::string name;
  std::uint16_t handle;
};


class Pcan_usb_transceiver final : public QObject
{
  Q_OBJECT

public:
  void start(const Pcan_channel& channel, Pcan_baudrate baudrate);
  void run(const Pcan_channel& channel, Pcan_baudrate baudrate);
  void stop();

  std::vector<Pcan_channel> channels();
  bool is_alive() const { return is_alive_.load(); }
  bool is_connected() const { return is_connected_.load(); }

signals:
  void received_frame(std::uint64_t, tin::Can_raw_frame);
  void started();
  void stopped();
  void connected();
  void disconnected();

private:
  std::atomic<bool> is_alive_ = false;
  std::atomic<bool> is_connected_ = false;
};


}  // namespace network


#endif  // NETWORK_PCAN_USB_TRANSCEIVER_H
