#ifndef TIN_BUSSIGNALDEF_H
#define TIN_BUSSIGNALDEF_H


#include <cstdint>
#include <string>


namespace tin
{


enum class Byte_order { Intel, Moto };
enum class Value_sign { Signed, Unsigned };


struct Bus_signal_def
{
  Byte_order order;
  Value_sign sign;
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
