#include "pcanusbtransceiver.h"


#include <array>
#include <windows.h>
#include "fmt/fmtlib.h"
#include "pcan-usb/PCANBasic.h"


void network::Pcan_usb_transceiver::start(const Pcan_channel& channel, Pcan_baudrate baudrate)
{
  if (!is_alive_.load()) {
    std::thread{&network::Pcan_usb_transceiver::run, this, channel, baudrate}.detach();
  }
}


void network::Pcan_usb_transceiver::run(const Pcan_channel& channel, Pcan_baudrate baudrate)
{
  constexpr std::uint32_t CAN_FLAG_EXT = 1u << 31;
  constexpr std::uint32_t CAN_FLAG_RTR = 1u << 30;
  constexpr std::uint32_t CAN_FLAG_ERR = 1u << 29;

  is_alive_.store(true);
  emit started();

  TPCANBaudrate rate = PCAN_BAUD_500K;

  switch (baudrate) {
    case Pcan_baudrate::Baud_100k: rate = PCAN_BAUD_100K; break;
    case Pcan_baudrate::Baud_125k: rate = PCAN_BAUD_125K; break;
    case Pcan_baudrate::Baud_250k: rate = PCAN_BAUD_250K; break;
    case Pcan_baudrate::Baud_500k: rate = PCAN_BAUD_500K; break;
    case Pcan_baudrate::Baud_800k: rate = PCAN_BAUD_800K; break;
    case Pcan_baudrate::Baud_1m: rate = PCAN_BAUD_1M; break;
  }

  TPCANStatus result = CAN_Initialize(channel.handle, rate);

  if (result == PCAN_ERROR_OK) {
    is_connected_.store(true);
    emit connected();
  }
  else {
    is_alive_.store(false);
    emit stopped();
    return;
  }

  CAN_Reset(channel.handle);

  while (is_alive_.load()) {
    TPCANMsg message;
    TPCANTimestamp timestamp;
    TPCANStatus status = CAN_Read(channel.handle, &message, &timestamp);

    while (status == PCAN_ERROR_OK) {
      tin::Can_raw_frame frame;
      frame.id = message.ID;
      frame.dlc = message.LEN;
      std::memcpy(frame.data.data(), message.DATA, message.LEN);

      if (message.MSGTYPE & PCAN_MESSAGE_EXTENDED) { frame.id |= CAN_FLAG_EXT; }
      if (message.MSGTYPE & PCAN_MESSAGE_RTR) { frame.id |= CAN_FLAG_RTR; }
      if (message.MSGTYPE & PCAN_MESSAGE_ERRFRAME) { frame.id |= CAN_FLAG_ERR; }

      std::uint64_t time = static_cast<std::uint64_t>(timestamp.millis);
      emit received_frame(time, frame);

      status = CAN_Read(channel.handle, &message, &timestamp);
    }

    if (status == PCAN_ERROR_QRCVEMPTY) {
      std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }
    else {
      break;
    }
  }

  CAN_Uninitialize(channel.handle);

  is_connected_.store(false);
  emit disconnected();
  emit stopped();
}


void network::Pcan_usb_transceiver::stop()
{
  is_alive_.store(false);
}


void network::Pcan_usb_transceiver::transmit(const tin::Can_raw_frame& frame)
{
  if (!is_connected_.load()) {
    return;
  }
}


auto network::Pcan_usb_transceiver::channels() -> std::vector<Pcan_channel>
{
  std::array<TPCANChannelInformation, 16> channels;
  DWORD n = sizeof(TPCANChannelInformation) * channels.size();

  auto status = CAN_GetValue(PCAN_NONEBUS, PCAN_ATTACHED_CHANNELS, &channels, n);

  if (status != PCAN_ERROR_OK) {
    return {};
  }

  int channel_count = n / sizeof(TPCANChannelInformation);
  std::vector<Pcan_channel> pcan_channels;

  for (int i=0; i<channel_count; ++i) {
    const auto handle = static_cast<uint16_t>(channels[i].channel_handle);

    if (handle >= PCAN_USBBUS1 && handle <= PCAN_USBBUS8) {
      pcan_channels.push_back({fmt::format("Channel {}", handle - PCAN_USBBUS1 + 1), handle});
    }
  }

  return pcan_channels;
}
