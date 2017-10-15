#include "bussignaldefitem.h"


#include <QString>
#include "tincan/bussignaldef.h"


QVariant tin::Bus_signal_def_item::data(int column) const
{
  switch (column) {
    case 0: return QString::fromStdString(bus_signal_def_->name);
    case 1: return bus_signal_def_->pos;
    case 2: return bus_signal_def_->len;
    case 3: {
      if (bus_signal_def_->multiplex_switch)
        return "switch";
      else if (bus_signal_def_->multiplex_value != -1)
        return bus_signal_def_->multiplex_value;
      return "";
    }
    case 4: return bus_signal_def_->sign == tin::Value_sign::Unsigned ? "unsigned" : "signed";
    case 5: return bus_signal_def_->order == tin::Byte_order::Intel ? "intel" : "moto";
    case 6: return bus_signal_def_->factor;
    case 7: return bus_signal_def_->offset;
    case 8: return bus_signal_def_->minimum;
    case 9: return bus_signal_def_->maximum;
    case 10: return QString::fromLocal8Bit(bus_signal_def_->unit.c_str());
  }

  return QVariant{};
}
