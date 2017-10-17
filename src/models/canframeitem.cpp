#include "canframeitem.h"


#include <QString>

#include "util.h"
#include "tincan/canframe.h"


QVariant tin::Can_frame_item::data(int column) const
{
  switch (column) {
    case 0: return can_frame_->frame_def ? QString::fromStdString(can_frame_->frame_def->name) : "FRAME";
    case 1: return QString::number(can_frame_->id, 16).toUpper();
    case 2: return can_frame_->receive_time;
    case 3: return QString::number(util::math::mean(can_frame_->cycle_times) / 1000.0, 'f', 3);
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
