#ifndef TIN_HELPER_H
#define TIN_HELPER_H


#include <cstdint>
#include <array>
#include <variant>

#include "tincan/cansignal.h"
#include "tincan/canframe.h"


namespace tin {


inline const Can_signal* find_signal(const Can_frame* frame, std::string_view name)
{
  if (!frame)
    return nullptr;

  auto it = std::find_if(std::begin(frame->can_signals), std::end(frame->can_signals),
      [name](const auto& s){ return s.signal_def && s.signal_def->name == name; });
  if (it != std::end(frame->can_signals))
    return &*it;
  return nullptr;
}


}  // namespace tin


#endif  // TIN_HELPER_H
