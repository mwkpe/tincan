#ifndef TIN_CANSIGNAL_H
#define TIN_CANSIGNAL_H


#include <cstdint>
#include <variant>

#include "tincan/cansignaldef.h"


namespace tin {


struct Can_signal
{
  std::variant<std::uint64_t, std::int64_t> raw;
  std::variant<std::uint64_t, std::int64_t, double> phys;
  const Can_signal_def* signal_def;
};


}  // namespace tin


#endif  // TIN_CANSIGNAL_H
