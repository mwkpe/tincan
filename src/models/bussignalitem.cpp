#include "bussignalitem.h"


#include <QString>


QVariant tin::Bus_signal_item::data(int column) const
{
  switch (column) {
    case 0: return bus_signal_.signal_def ? QString::fromStdString(bus_signal_.signal_def->name)
        : QVariant{};
  }

  return QVariant{};
}
