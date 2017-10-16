#include "cantracer.h"


#include "fmt/fmtlib.h"

#include "canframe.h"
#include "canbus.h"


namespace
{


const auto trace_format = R"({},{:0>4},)"
    R"(<font color="#33E7F7">{:0>8X}</font>,<font color="#23E02B">{}</font>,)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(<font color="{}">{:0>2X}</font>)"
    R"(,<font color="#FBED27">{}</font>)";


}  // namespace


void tin::Can_tracer::update_data(std::uint32_t id)
{
  if (trace_frame_ && id == trace_frame_->id) {
    auto line = QString::fromStdString(fmt::format(trace_format,
        trace_frame_->receive_time,
        prev_frame_.receive_time > 0 ? trace_frame_->receive_time - prev_frame_.receive_time : 0,
        trace_frame_->id,
        trace_frame_->frame_def ? trace_frame_->frame_def->dlc : 8,
        trace_frame_->raw_data[7] != prev_frame_.raw_data[7] ? "#FB4FE7" : "#C0C0C0",
        trace_frame_->raw_data[7],
        trace_frame_->raw_data[6] != prev_frame_.raw_data[6] ? "#FB4FE7" : "#C0C0C0",
        trace_frame_->raw_data[6],
        trace_frame_->raw_data[5] != prev_frame_.raw_data[5] ? "#FB4FE7" : "#C0C0C0",
        trace_frame_->raw_data[5],
        trace_frame_->raw_data[4] != prev_frame_.raw_data[4] ? "#FB4FE7" : "#C0C0C0",
        trace_frame_->raw_data[4],
        trace_frame_->raw_data[3] != prev_frame_.raw_data[3] ? "#FB4FE7" : "#C0C0C0",
        trace_frame_->raw_data[3],
        trace_frame_->raw_data[2] != prev_frame_.raw_data[2] ? "#FB4FE7" : "#C0C0C0",
        trace_frame_->raw_data[2],
        trace_frame_->raw_data[1] != prev_frame_.raw_data[1] ? "#FB4FE7" : "#C0C0C0",
        trace_frame_->raw_data[1],
        trace_frame_->raw_data[0] != prev_frame_.raw_data[0] ? "#FB4FE7" : "#C0C0C0",
        trace_frame_->raw_data[0],
        trace_frame_->frame_def ? trace_frame_->frame_def->name : "FRAME"));
        prev_frame_ = *trace_frame_;
    emit line_ready(line);
  }
}
