#include "canbus.h"


#include "canbusdef.h"
#include "signalutil.h"


namespace
{


void calculate_signal_values(tin::Can_frame& frame)
{
  for (auto& signal : frame.bus_signals) {
    const auto* def = signal.signal_def;
    signal.raw = tin::build_raw_value(frame.raw_data, def->pos, def->len, def->order, def->sign);
    signal.phys = tin::calc_phys_value(signal.raw, def->factor, def->offset);
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
    auto& prev_time = prev_frame_time_[frame.id];
    frame.cycle_times.push_back(time - prev_time);
    prev_time = time;
    if (frame.frame_def)
      calculate_signal_values(frame);
  }
  else {  // A new frame, add to bus
    Can_frame frame;
    frame.id = raw_frame.id;
    frame.raw_data = raw_frame.data;
    frame.receive_time = time;
    prev_frame_time_[frame.id] = time;
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

  emit data_changed(raw_frame.id);
}
