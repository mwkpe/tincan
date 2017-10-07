#include "canbus.h"


std::tuple<std::uint64_t, can::Frame> can::Bus::frame(std::uint32_t id) const
{
  auto it = frames_.find(id);
  if (it != std::end(frames_)) {
    return it->second;
  }

  return {0, can::Frame{}};
}


void can::Bus::add_frame(std::uint64_t time, can::Frame frame)
{
  frames_[frame.id] = std::make_tuple(time, frame);
}
