#include "bussignalitem.h"


#include <cstdint>
#include <QString>
#include "tincan/bussignal.h"


QVariant tin::Bus_signal_item::data(int column) const
{
  switch (column) {
    case 0: return QString::fromStdString(bus_signal_->signal_def->name);
    case 1:
      if (std::holds_alternative<std::uint64_t>(bus_signal_->phys))
        return std::get<std::uint64_t>(bus_signal_->phys);
      else if (std::holds_alternative<std::int64_t>(bus_signal_->phys))
        return std::get<std::int64_t>(bus_signal_->phys);
      return std::get<double>(bus_signal_->phys);
    case 2:
      if (std::holds_alternative<std::uint64_t>(bus_signal_->raw))
        return std::get<std::uint64_t>(bus_signal_->raw);
      return std::get<std::int64_t>(bus_signal_->raw);
  }

  return QVariant{};
}
