#include "canbus.h"


#include <algorithm>

#include "tincan/canbusdef.h"
#include "tincan/signalutil.h"
#include "util.h"


namespace {


void calculate_signal_values(tin::Can_frame& frame)
{
  if (frame.frame_def->multiplexer) {
    std::int32_t switch_value = -1;
    auto it = std::find_if(std::begin(frame.can_signals), std::end(frame.can_signals),
        [&](auto&& s) { return s.signal_def->multiplex_switch; });
    if (it != std::end(frame.can_signals)) {
      const auto* def = it->signal_def;
      auto raw = tin::build_raw_value(frame.raw_data, def->pos, def->len, def->order, def->sign);
      switch_value = static_cast<std::int32_t>(std::get<std::uint64_t>(raw));
    }
    for (auto& signal : frame.can_signals) {
      const auto* def = signal.signal_def;
      if (def->multiplex_value == -1 || def->multiplex_value == switch_value) {
        signal.raw = tin::build_raw_value(frame.raw_data, def->pos, def->len, def->order, def->sign);
        signal.phys = tin::calc_phys_value(signal.raw, def->factor, def->offset);
      }
    }
  }
  else {
    for (auto& signal : frame.can_signals) {
      const auto* def = signal.signal_def;
      signal.raw = tin::build_raw_value(frame.raw_data, def->pos, def->len, def->order, def->sign);
      signal.phys = tin::calc_phys_value(signal.raw, def->factor, def->offset);
    }
  }
}


}  // namespace


void tin::Can_bus::reset()
{
  set_definition(nullptr);
  reset_frames();
}


const tin::Can_frame* tin::Can_bus::frame(std::uint32_t id) const
{
  auto it = frames_.find(id);
  if (it != std::end(frames_))
    return &it->second;
  return nullptr;
}


void tin::Can_bus::add_frame(std::uint64_t time, tin::Can_raw_frame raw_frame)
{
  constexpr std::uint32_t CAN_FLAG_EXT = 1u << 31;
  constexpr std::uint32_t CAN_FLAG_RTR = 1u << 30;
  constexpr std::uint32_t CAN_FLAG_ERR = 1u << 29;

  auto it = frames_.find(raw_frame.id & 0x1fffffff);
  if (it != std::end(frames_)) {  // Frame was received previously, update data
    auto& frame = it->second;
    frame.raw_data = raw_frame.data;
    frame.extended = (raw_frame.id & CAN_FLAG_EXT) != 0;
    frame.rtr = (raw_frame.id & CAN_FLAG_RTR) != 0;
    frame.error = (raw_frame.id & CAN_FLAG_ERR) != 0;
    frame.receive_time = time;
    frame.last_receive_system_time = util::Timer::system_now();
    frame.alive = true;
    auto& prev_time = prev_frame_time_[frame.id];
    auto& cycle_times = cycle_times_[frame.id];
    cycle_times.push_back(static_cast<std::int32_t>(time - prev_time));
    frame.mean_cycle_time = util::math::mean(cycle_times);
    prev_time = time;
    if (frame.frame_def)
      calculate_signal_values(frame);
  }
  else {  // A new frame, add to bus
    Can_frame frame;
    frame.id = raw_frame.id & 0x1fffffff;
    frame.length = raw_frame.dlc;
    frame.raw_data = raw_frame.data;
    frame.extended = (raw_frame.id & CAN_FLAG_EXT) != 0;
    frame.rtr = (raw_frame.id & CAN_FLAG_RTR) != 0;
    frame.error = (raw_frame.id & CAN_FLAG_ERR) != 0;
    frame.receive_time = time;
    frame.last_receive_system_time = util::Timer::system_now();
    frame.alive = true;
    prev_frame_time_[frame.id] = time;
    cycle_times_[frame.id] = boost::circular_buffer<std::int32_t>{20};
    if (bus_def_) {
      if (frame.frame_def = find_frame_def(*bus_def_, raw_frame.id); frame.frame_def) {
        for (const auto& signal_def : frame.frame_def->can_signal_defs) {
          frame.can_signals.emplace_back();
          frame.can_signals.back().signal_def = &signal_def;
        }
        calculate_signal_values(frame);
      }
    }
    frames_[frame.id] = frame;
  }

  emit data_changed(raw_frame.id);
}


void tin::Can_bus::update_frames()
{
  auto now = util::Timer::system_now();
  for (auto& p : frames_) {
    auto& frame = p.second;
    if (frame.alive) {
      auto delta = now - frame.last_receive_system_time;
      if (frame.mean_cycle_time > 0 && delta > frame.mean_cycle_time * 3) {
        frame.alive = false;
        emit data_changed(frame.id);
      }
    }
  }
}
