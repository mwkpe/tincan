#include "signalutil.h"


#include <cstring>
#include "bussignaldef.h"


std::variant<uint64_t, int64_t> tin::raw_value(const std::array<std::uint8_t, 8>& buffer,
    std::uint32_t pos, std::uint32_t len, Byte_order order, Value_sign sign)
{
  std::uint64_t raw;
  std::memcpy(&raw, &buffer, sizeof(raw));

  if (order == tin::Byte_order::Moto) {
    raw = __builtin_bswap64(raw);
    pos = pswap64(pos) - len + 1;  // Set position to least significant bit
  }

  // Using unsigned since shift operations on signed values aren't well-defined
  raw <<= 64 - (pos + len);
  if (sign == tin::Value_sign::Signed)
    raw = ~raw;  // Prepare value for zero-insertions due to right shift
  raw >>= 64 - len;
  if (sign == tin::Value_sign::Signed) {
    raw = ~raw;  // Flipping back to proper value (all inserted zeros are now one)
    std::int64_t raw_signed;
    std::memcpy(&raw_signed, &raw, sizeof(raw_signed));
    return raw_signed;
  }
  return raw;
}
