#ifndef TIN_BUSSIGNALDEF_H
#define TIN_BUSSIGNALDEF_H


#include <cstdint>
#include <string>


namespace tin
{


enum class Byte_order : std::uint8_t { Intel, Moto };
enum class Value_sign : std::uint8_t { Signed, Unsigned };


struct Bus_signal_def
{
  bool multiplex_switch;
  Byte_order order;
  Value_sign sign;
  std::int32_t multiplex_value;
  std::uint32_t pos;
  std::uint32_t len;
  double factor;
  double offset;
  double minimum;
  double maximum;
  std::string unit;
  std::string name;
};


}  // namespace tin


#endif  // TIN_BUSSIGNALDEF_H
