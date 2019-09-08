#ifndef TIN_CANFRAME_H
#define TIN_CANFRAME_H


#include <cstdint>
#include <array>
#include <vector>
#include <boost/circular_buffer.hpp>
#include "bussignal.h"
#include "canframedef.h"


namespace tin
{


struct Can_frame
{
  std::uint32_t id = 0;
  std::uint64_t receive_time = 0;
  std::int64_t last_receive_system_time = 0;
  std::int32_t mean_cycle_time = 0;
  std::array<std::uint8_t, 8> raw_data;
  std::vector<Bus_signal> bus_signals;
  const Can_frame_def* frame_def = nullptr;
  bool alive = false;
  boost::circular_buffer<std::int32_t> cycle_times{32};  // For average calculation
};


}  // namespace tin


#endif  // TIN_CANFRAME_H
