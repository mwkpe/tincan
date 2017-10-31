#include "signalutil.h"


#include <cstring>
#include <cmath>

#ifdef _MSC_VER
  #include <stdlib.h>
#endif

#include "bussignaldef.h"


namespace
{


inline bool is_fraction(double val)
{
  double integral;
  auto fractional = std::modf(val, &integral);
  return std::abs(fractional) != 0;
}


template<typename T> tin::phys_val calc_phys(T val, double factor, double offset)
{
  // Nothing to be done, raw and physical value are identical
  if (factor == 1 && offset == 0)
    return val;

  // We need a floating point value to represent the result
  if (is_fraction(factor) || is_fraction(offset))
    return static_cast<double>(val) * factor + offset;

  // Let's squeeze the result into an integer and hope it fits (sue me)
  return static_cast<std::int64_t>(val * factor + offset);
}
template tin::phys_val calc_phys(std::uint64_t, double, double);
template tin::phys_val calc_phys(std::int64_t, double, double);


}  // namespace


tin::raw_val tin::build_raw_value(const std::array<std::uint8_t, 8>& buffer, std::uint32_t pos,
    std::uint32_t len, Byte_order order, Value_sign sign)
{
  std::uint64_t raw;
  std::memcpy(&raw, &buffer, sizeof(raw));

  if (order == tin::Byte_order::Moto) {
#ifdef __GNUC__
    raw = __builtin_bswap64(raw);
#elif _MSC_VER
    raw = _byteswap_uint64(raw);
#else
    #error "Byte swap required"
#endif
    pos = pswap64(pos) - len + 1;  // Set position to least significant bit
  }

  // Using unsigned since shift operations on signed values aren't well-defined
  raw <<= 64 - (pos + len);
  bool is_negative = raw & (1ull << 63);
  if (sign == tin::Value_sign::Signed && is_negative)
    raw = ~raw;  // Prepare negative values for zero-insertions due to right shift
  raw >>= 64 - len;
  if (sign == tin::Value_sign::Signed) {
    if (is_negative)
      raw = ~raw;  // Flipping back to proper value (all inserted zeros are now one)
    std::int64_t raw_signed;
    std::memcpy(&raw_signed, &raw, sizeof(raw_signed));
    return raw_signed;
  }
  return raw;
}


tin::phys_val tin::calc_phys_value(raw_val raw, double factor, double offset)
{
  if (std::holds_alternative<std::uint64_t>(raw))
    return calc_phys(std::get<std::uint64_t>(raw), factor, offset);
  return calc_phys(std::get<std::int64_t>(raw), factor, offset);
}
