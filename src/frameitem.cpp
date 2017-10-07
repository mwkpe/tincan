#include "frameitem.h"


#include <QString>

#include "canframe.h"
#include "dbcfile.h"


QVariant tin::Frame_item::data(int column) const
{
  switch (column) {
    case 0: return frame_def_ ? QString::fromStdString(frame_def_->name) : "FRAME";
    case 1: return frame_.id;
    case 2: return counter_;
    case 3: {
      QString hex_data;
      for (auto it = std::rbegin(frame_.data); it != std::rend(frame_.data); ++it) {
        hex_data += QString::number(*it, 16).rightJustified(2, '0');
        hex_data += " ";
      }
      return hex_data.toUpper();
    }
  }

  return QVariant{};
}
