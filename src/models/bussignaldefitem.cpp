#include "bussignaldefitem.h"


#include <QString>
#include "tincan/bussignaldef.h"


QVariant tin::Bus_signal_def_item::data(int column) const
{
  const auto* signal = bus_signal_def_;
  switch (column) {
    case 0: return QString::fromStdString(signal->name);
    case 1: return signal->pos;
    case 2: return signal->len;
    case 3: {
      if (signal->multiplex_switch)
        return "S";
      else if (signal->multiplex_value != -1)
        return signal->multiplex_value;
      return "";
    }
    case 4: return signal->sign == tin::Value_sign::Unsigned ? "unsigned" : "signed";
    case 5: return signal->order == tin::Byte_order::Intel ? "intel" : "moto";
    case 6: return QString::number(signal->factor, 'f', signal->meta_data.factor_precision);
    case 7: return QString::number(signal->offset, 'f', signal->meta_data.offset_precision);
    case 8: return QString::number(signal->minimum, 'f', signal->meta_data.minimum_precision);
    case 9: return QString::number(signal->maximum, 'f', signal->meta_data.maximum_precision);
    case 10: return QString::fromLocal8Bit(signal->unit.c_str());
  }

  return QVariant{};
}
