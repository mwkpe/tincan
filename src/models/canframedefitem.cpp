#include "canframedefitem.h"


#include <QString>


QVariant tin::Can_frame_def_item::data(int column) const
{
  switch (column) {
    case 0: return QString::fromStdString(can_frame_def_->name);
    case 1: return QString::number(can_frame_def_->id, 16).toUpper() + "h";
    case 2: return can_frame_def_->dlc;
  }

  return QVariant{};
}
