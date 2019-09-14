#include "cansignalitem.h"


#include <cstdint>
#include <QString>
#include "tincan/cansignal.h"


QVariant tin::Can_signal_item::data(int column) const
{
  switch (column) {
    case 0: return QString::fromStdString(can_signal_->signal_def->name);
    case 1: {
      if (!can_signal_->signal_def->value_definitions.empty()) {
        std::int32_t index = 0;
        // Slice and dice
        if (std::holds_alternative<std::uint64_t>(can_signal_->raw))
          index = static_cast<std::int32_t>(std::get<std::uint64_t>(can_signal_->raw));
        else
          index = static_cast<std::int32_t>(std::get<std::int64_t>(can_signal_->raw));
        const auto& value_defs = can_signal_->signal_def->value_definitions;
        if (auto it = value_defs.find(index); it != std::end(value_defs))
          return QString::fromStdString(it->second);
      }

      if (std::holds_alternative<std::uint64_t>(can_signal_->phys)) {
        return std::get<std::uint64_t>(can_signal_->phys);
      }
      else if (std::holds_alternative<std::int64_t>(can_signal_->phys)) {
        return std::get<std::int64_t>(can_signal_->phys);
      }
      else {
        return QString::number(std::get<double>(can_signal_->phys), 'f',
            can_signal_->signal_def->meta_data.factor_precision);
      }
    }
    case 2: return QString::fromStdString(can_signal_->signal_def->unit);
    case 5:
      if (std::holds_alternative<std::uint64_t>(can_signal_->raw))
        return std::get<std::uint64_t>(can_signal_->raw);
      return std::get<std::int64_t>(can_signal_->raw);
  }

  return QVariant{};
}
