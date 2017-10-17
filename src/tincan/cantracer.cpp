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


}  // namespace


void tin::Can_tracer::update_data(std::uint32_t id)
{
  if (trace_frame_ && !paused_ && id == trace_frame_->id) {
    QString line;
    if (trace_signal_) {
      std::string physical_value;
      if (std::holds_alternative<std::uint64_t>(trace_signal_->phys))
        physical_value = fmt::format("{:>10}", std::get<std::uint64_t>(trace_signal_->phys));
      else if (std::holds_alternative<std::int64_t>(trace_signal_->phys))
        physical_value = fmt::format("{:>10}", std::get<std::int64_t>(trace_signal_->phys));
      else
        physical_value = fmt::format("{:>10f}", std::get<double>(trace_signal_->phys));

      line = QString::fromStdString(fmt::format(signal_format,
          trace_frame_->receive_time,
          prev_frame_.receive_time > 0 ? trace_frame_->receive_time - prev_frame_.receive_time : 0,
          physical_value,
          std::holds_alternative<std::uint64_t>(trace_signal_->raw) ?
              std::get<std::uint64_t>(trace_signal_->raw) : std::get<std::int64_t>(trace_signal_->raw),
          trace_signal_->signal_def->name));
    }
    else {
      line = QString::fromStdString(fmt::format(frame_format,
          trace_frame_->receive_time,
          prev_frame_.receive_time > 0 ? trace_frame_->receive_time - prev_frame_.receive_time : 0,
          trace_frame_->id,
          trace_frame_->frame_def ? trace_frame_->frame_def->dlc : 8,
          trace_frame_->raw_data[7] != prev_frame_.raw_data[7] ? magenta : silver,
          trace_frame_->raw_data[7],
          trace_frame_->raw_data[6] != prev_frame_.raw_data[6] ? magenta : silver,
          trace_frame_->raw_data[6],
          trace_frame_->raw_data[5] != prev_frame_.raw_data[5] ? magenta : silver,
          trace_frame_->raw_data[5],
          trace_frame_->raw_data[4] != prev_frame_.raw_data[4] ? magenta : silver,
          trace_frame_->raw_data[4],
          trace_frame_->raw_data[3] != prev_frame_.raw_data[3] ? magenta : silver,
          trace_frame_->raw_data[3],
          trace_frame_->raw_data[2] != prev_frame_.raw_data[2] ? magenta : silver,
          trace_frame_->raw_data[2],
          trace_frame_->raw_data[1] != prev_frame_.raw_data[1] ? magenta : silver,
          trace_frame_->raw_data[1],
          trace_frame_->raw_data[0] != prev_frame_.raw_data[0] ? magenta : silver,
          trace_frame_->raw_data[0],
          trace_frame_->frame_def ? trace_frame_->frame_def->name : "FRAME"));
    }
    prev_frame_ = *trace_frame_;
    emit line_ready(line);
  }
}
