#include "cansignaldefitem.h"


#include <QString>
#include "tincan/cansignaldef.h"


QVariant tin::Can_signal_def_item::data(int column) const
{
  const auto* s = can_signal_def_;
  switch (column) {
    case 0: return QString::fromStdString(s->name);
    case 1: return s->pos;
    case 2: return s->len;
    case 3: {
      if (s->multiplex_switch)
        return "S";
      else if (s->multiplex_value != -1)
        return s->multiplex_value;
      return "";
    }
    case 4: return s->sign == tin::Value_sign::Unsigned ? "unsigned" : "signed";
    case 5: return s->order == tin::Byte_order::Intel ? "intel" : "moto";
    case 6: return QString::number(s->factor, 'f', s->meta_data.factor_precision);
    case 7: return QString::number(s->offset, 'f', s->meta_data.offset_precision);
    case 8: return QString::number(s->minimum, 'f', s->meta_data.minimum_precision);
    case 9: return QString::number(s->maximum, 'f', s->meta_data.maximum_precision);
    case 10: return QString::fromLocal8Bit(s->unit.c_str());
  }

  return QVariant{};
}
