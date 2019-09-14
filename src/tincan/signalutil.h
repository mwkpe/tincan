#ifndef TIN_SIGNALUTIL_H
#define TIN_SIGNALUTIL_H


#include <cstdint>
#include <array>
#include <variant>

#include "tincan/cansignaldef.h"


namespace tin {


using raw_val = std::variant<uint64_t, int64_t>;
using phys_val = std::variant<uint64_t, int64_t, double>;


inline std::uint32_t pswap64(std::uint32_t pos)
{
  // Recalculate bit position after bytes were swapped by __builtin_bswap64
  return 64ul - ((pos / 8 + 1) * 8) + (pos % 8);
}

raw_val build_raw_value(const std::array<std::uint8_t, 8>& raw_bytes, std::uint32_t pos,
    std::uint32_t len, Byte_order order, Value_sign sign);

phys_val calc_phys_value(raw_val raw, double factor, double offset);


}  // namespace tin


#endif  // TIN_SIGNALUTIL_H
