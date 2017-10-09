#ifndef TIN_CANFRAME_H
#define TIN_CANFRAME_H


#include <cstdint>
#include <array>
#include <vector>

#include "bussignal.h"
#include "canframedef.h"


namespace tin
{


struct Can_frame
{
  std::uint32_t id;
  std::uint64_t receive_time;
  std::array<std::uint8_t, 8> raw_data;
  std::vector<Bus_signal> bus_signals;
  const Can_frame_def* frame_def = nullptr;
};


}  // namespace tin


#endif  // TIN_CANFRAME_H
