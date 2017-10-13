#include "canbus.h"


#include <array>
#include <cstring>
#include <algorithm>

#include "bussignaldef.h"
#include "canbusdef.h"


namespace
{


std::uint32_t lsbit_intel(std::uint32_t msbit_moto, std::uint32_t len)
{
  // Recalculate position after swapping bytes
  return 64ul - ((msbit_moto / 8 + 1) * 8) + (msbit_moto % 8) - len + 1;
}


std::variant<uint64_t, int64_t> raw_value(const std::array<std::uint8_t, 8>& raw_bytes,
    std::uint32_t pos, std::uint32_t len, tin::Byte_order order, tin::Value_sign sign)
{
  std::uint64_t raw;
  std::memcpy(&raw, &raw_bytes, sizeof(raw));

  if (order == tin::Byte_order::Moto) {
    raw = __builtin_bswap64(raw);
    pos = lsbit_intel(pos, len);
  }

  // Using unsigned since shift operations on signed values aren't well-defined
  raw <<= 64 - (pos + len);
  if (sign == tin::Value_sign::Signed)
    raw = ~raw;  // Prepare value for zero-insertions due to right shift
  raw >>= 64 - len;
  if (sign == tin::Value_sign::Signed) {
    raw = ~raw;  // Flipping back to proper value (all inserted zeros are now one)
    std::int64_t raw_signed;
    std::memcpy(&raw_signed, &raw, sizeof(raw_signed));
    return raw_signed;
  }
  return raw;
}


void calculate_signal_values(tin::Can_frame& frame)
{
  for (auto& signal : frame.bus_signals) {
    auto* signal_def = signal.signal_def;
    signal.raw = raw_value(frame.raw_data, signal_def->pos, signal_def->len, signal_def->order,
        signal_def->sign);
  }
}


}  // namespace


const tin::Can_frame* tin::Can_bus::frame(std::uint32_t id) const
{
  auto it = frames_.find(id);
  if (it != std::end(frames_))
    return &it->second;
  return nullptr;
}


void tin::Can_bus::add_frame(std::uint64_t time, can::Raw_frame raw_frame)
{
  auto it = frames_.find(raw_frame.id);
  if (it != std::end(frames_)) {  // Frame was received previously, update data
    auto& frame = it->second;
    frame.raw_data = raw_frame.data;
    frame.receive_time = time;
    if (frame.frame_def)
      calculate_signal_values(frame);
  }
  else {  // A new frame, add to bus
    Can_frame frame;
    frame.id = raw_frame.id;
    frame.raw_data = raw_frame.data;
    frame.receive_time = time;
    if (bus_def_) {
      if (frame.frame_def = find_frame_def(*bus_def_, raw_frame.id); frame.frame_def) {
        for (const auto& signal_def : frame.frame_def->bus_signal_defs) {
          frame.bus_signals.emplace_back();
          frame.bus_signals.back().signal_def = &signal_def;
        }
        calculate_signal_values(frame);
      }
    }
    frames_[frame.id] = frame;
  }
}
