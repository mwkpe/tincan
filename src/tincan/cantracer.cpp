#include "cantracer.h"


#include "fmt/fmtlib.h"

#include "bussignal.h"
#include "canframe.h"
#include "canbus.h"


namespace
{


using namespace std::string_literals;


const auto silver = "#C0C0C0"s;
const auto magenta = "#FB4FE7"s;


const auto frame_format = R"(<p style="white-space:pre"><font color="#808080">{}</font>,{:>4},)"
    R"(<font color="#33E7F7">{:X}</font>,)"
    R"(<font color="#23E02B">{}</font>,)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(,<font color="#FBED27">{}</font></p>)";

const auto signal_format = R"(<p style="white-space:pre"><font color="#808080">{}</font>,{:>4},)"
    R"(<font color="#33E7F7">{}</font>,)"
    R"(<font color="#23E02B">{:>10}</font>,)"
    R"(<font color="#FBED27">{}</font></p>)";


std::string to_string(const tin::Can_frame* frame, const tin::Can_frame* prev_frame)
{
  return fmt::format(frame_format,
      frame->receive_time,
      prev_frame->receive_time > 0 ? frame->receive_time - prev_frame->receive_time : 0,
      frame->id,
      frame->frame_def ? frame->frame_def->dlc : 8,
      frame->raw_data[7] != prev_frame->raw_data[7] ? magenta : silver,
      frame->raw_data[7],
      frame->raw_data[6] != prev_frame->raw_data[6] ? magenta : silver,
      frame->raw_data[6],
      frame->raw_data[5] != prev_frame->raw_data[5] ? magenta : silver,
      frame->raw_data[5],
      frame->raw_data[4] != prev_frame->raw_data[4] ? magenta : silver,
      frame->raw_data[4],
      frame->raw_data[3] != prev_frame->raw_data[3] ? magenta : silver,
      frame->raw_data[3],
      frame->raw_data[2] != prev_frame->raw_data[2] ? magenta : silver,
      frame->raw_data[2],
      frame->raw_data[1] != prev_frame->raw_data[1] ? magenta : silver,
      frame->raw_data[1],
      frame->raw_data[0] != prev_frame->raw_data[0] ? magenta : silver,
      frame->raw_data[0],
      frame->frame_def ? frame->frame_def->name : "FRAME");
}


std::string to_string(const tin::Can_frame* frame, const tin::Can_frame* prev_frame,
    const tin::Bus_signal* signal)
{
  std::string physical_value;
  if (std::holds_alternative<std::uint64_t>(signal->phys))
    physical_value = fmt::format("{:>10}", std::get<std::uint64_t>(signal->phys));
  else if (std::holds_alternative<std::int64_t>(signal->phys))
    physical_value = fmt::format("{:>10}", std::get<std::int64_t>(signal->phys));
  else
    physical_value = fmt::format("{:>10f}", std::get<double>(signal->phys));

  return fmt::format(signal_format,
      frame->receive_time,
      prev_frame->receive_time > 0 ? frame->receive_time - prev_frame->receive_time : 0,
      physical_value,
      std::holds_alternative<std::uint64_t>(signal->raw) ? std::get<std::uint64_t>(signal->raw) :
          std::get<std::int64_t>(signal->raw),
      signal->signal_def->name);
}


}  // namespace


void tin::Can_tracer::update_data(std::uint32_t id)
{
  if (trace_frame_ && !paused_ && id == trace_frame_->id) {
    QString line;
    if (trace_signal_)
      line = QString::fromStdString(to_string(trace_frame_, &prev_frame_, trace_signal_));
    else
      line = QString::fromStdString(to_string(trace_frame_, &prev_frame_));
    prev_frame_ = *trace_frame_;
    emit line_ready(line);
  }
}
