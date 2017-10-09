#ifndef TIN_BUSSIGNAL_H
#define TIN_BUSSIGNAL_H


#include <cstdint>
#include <variant>

#include "bussignaldef.h"


namespace tin
{


struct Bus_signal
{
  std::variant<std::uint64_t, std::int64_t> raw;
  std::variant<std::uint64_t, std::int64_t, double> phys;
  const Bus_signal_def* signal_def;
};


}  // namespace tin


#endif  // TIN_BUSSIGNAL_H
