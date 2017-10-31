#ifndef TIN_BUSSIGNALDEF_H
#define TIN_BUSSIGNALDEF_H


#include <cstdint>
#include <string>
#include <vector>


namespace tin
{


enum class Byte_order : std::uint8_t { Intel, Moto };
enum class Value_sign : std::uint8_t { Signed, Unsigned };


struct Bus_signal_meta_data
{
  std::int8_t factor_precision = 7;
  std::int8_t offset_precision = 7;
  std::int8_t minimum_precision = 7;
  std::int8_t maximum_precision = 7;
};


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
  std::vector<std::string> receiver;
  Bus_signal_meta_data meta_data;
};


}  // namespace tin


#endif  // TIN_BUSSIGNALDEF_H
