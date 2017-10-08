#include "canframeitem.h"


#include <QString>


QVariant tin::Can_frame_item::data(int column) const
{
  switch (column) {
    case 0: return frame_.frame_def ? QString::fromStdString(frame_.frame_def->name) : "FRAME";
    case 1: return frame_.id;
    case 2: return frame_.receive_time;
    case 3: {
      QString hex_data;
      for (auto it = std::rbegin(frame_.raw_data); it != std::rend(frame_.raw_data); ++it) {
        hex_data += QString::number(*it, 16).rightJustified(2, '0');
        hex_data += " ";
      }
      return hex_data.toUpper();
    }
  }

  return QVariant{};
}
