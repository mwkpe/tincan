#include "canframeitem.h"


#include <numeric>
#include <iostream>
#include <QString>
#include "tincan/canframe.h"


void tin::Can_frame_item::update()
{
  if (cycle_times_pos_ >= cycle_times_.size()) {
    cycle_times_pos_ = 0;
//    for (auto i : cycle_times_)
//      std::cout << i << '\n';
  }

  auto delta_time = can_frame_->receive_time - last_frame_time_;
  cycle_times_[cycle_times_pos_] = delta_time;

  average_cycle_time_ = std::accumulate(std::begin(cycle_times_), std::end(cycle_times_), 0) /
      cycle_times_.size();
  last_frame_time_ = can_frame_->receive_time;
  cycle_times_pos_++;
}


QVariant tin::Can_frame_item::data(int column) const
{
  switch (column) {
    case 0: return can_frame_->frame_def ? QString::fromStdString(can_frame_->frame_def->name) : "FRAME";
    case 1: return QString::number(can_frame_->id, 16).toUpper();
    case 2: return can_frame_->receive_time;
    case 3: return QString::number(static_cast<double>(average_cycle_time_) / 1000.0, 'f', 3);
    case 4: {
      QString hex_data;
      for (auto it = std::rbegin(can_frame_->raw_data); it != std::rend(can_frame_->raw_data); ++it) {
        hex_data += QString::number(*it, 16).rightJustified(2, '0');
        hex_data += " ";
      }
      return hex_data.toUpper();
    }
  }

  return QVariant{};
}
