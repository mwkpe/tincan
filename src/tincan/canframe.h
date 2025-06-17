#ifndef TIN_CANFRAME_H
#define TIN_CANFRAME_H


#include <cstdint>
#include <array>
#include <vector>

#include "tincan/cansignal.h"
#include "tincan/canframedef.h"


namespace tin {


struct Can_frame
{
  std::uint32_t id = 0;
  std::int32_t mean_cycle_time = 0;
  std::uint64_t receive_time = 0;
  std::int64_t last_receive_system_time = 0;
  std::array<std::uint8_t, 8> raw_data;
  std::uint8_t length = 0;
  bool extended = false;
  bool rtr = false;
  bool error = false;
  std::vector<Can_signal> can_signals;
  const Can_frame_def* frame_def = nullptr;
  bool alive = false;
};


}  // namespace tin


#endif  // TIN_CANFRAME_H
