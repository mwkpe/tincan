#include "valuedefitem.h"



QVariant tin::Value_def_item::data(int column) const
{
  switch (column) {
    case 0: return value_;
    case 1: return definition_;
  }

  return QVariant{};
}
