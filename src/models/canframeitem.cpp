#include "canframeitem.h"


#include <QString>
#include "tincan/canframe.h"


QVariant tin::Can_frame_item::data(int column) const
{
  switch (column) {
    case 0: return can_frame_->frame_def ? QString::fromStdString(can_frame_->frame_def->name) : "FRAME";
    case 1: return QString{"0x"} + QString::number(can_frame_->id, 16).toUpper();
    case 2: return QVariant{}; //can_frame_->receive_time;
    case 3: return QString::number(can_frame_->mean_cycle_time / 1000.0, 'f', 3);
    case 4: return can_frame_->length;
    case 5: {
      QString hex_data;
      for (int i=can_frame_->length-1; i>=0 && i<8; --i) {
        hex_data += QString::number(can_frame_->raw_data[i], 16).rightJustified(2, '0');
        hex_data += " ";
      }
      return hex_data.toUpper();
    }
  }

  return QVariant{};
}
