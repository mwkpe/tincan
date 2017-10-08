#include "canbus.h"


#include "file/dbcfile.h"


std::tuple<bool, tin::Can_frame> tin::Can_bus::frame(std::uint32_t id) const
{
  auto it = frames_.find(id);
  if (it != std::end(frames_))
    return {true, it->second};
  return {false, Can_frame{}};
}


void tin::Can_bus::add_frame(std::uint64_t time, can::Raw_frame raw_frame)
{
  auto it = frames_.find(raw_frame.id);
  if (it != std::end(frames_)) {
    it->second.raw_data = raw_frame.data;
    it->second.receive_time = time;
  }
  else {
    Can_frame frame;
    frame.id = raw_frame.id;
    frame.raw_data = raw_frame.data;
    frame.receive_time = time;
    if (dbc_file_)
      frame.frame_def = dbc::find_frame_def(*dbc_file_, raw_frame.id);
    frames_[frame.id] = frame;
  }
}
