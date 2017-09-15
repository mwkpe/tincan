#ifndef DBC_PARSER_H
#define DBC_PARSER_H


#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>

#include <boost/fusion/include/adapt_struct.hpp>


namespace dbc
{


struct signal
{
  std::string name;
  std::int8_t pos;
  std::int8_t len;
  std::int8_t order;
  char sign;
  float factor;
  float offset;
  float minimum;
  float maximum;
};


struct message_base
{
  std::uint32_t id;
  std::int32_t cycle_time;
  std::string name;
  std::uint8_t dlc;
};


struct message : public message_base
{
  std::vector<signal> signals_;
};


struct file
{
  std::vector<message> messages;
};


class parse_error : public std::runtime_error
{
public:
  parse_error(const std::string& s) : std::runtime_error{s} {}
  parse_error(const char* s) : std::runtime_error{s} {}
};


file parse(std::string_view filename);


}  // namespace dbc


BOOST_FUSION_ADAPT_STRUCT(
  dbc::signal,
  (std::string, name)
  (std::int8_t, pos)
  (std::int8_t, len)
  (std::int8_t, order)
  (char, sign)
  (float, factor)
  (float, offset)
  (float, minimum)
  (float, maximum)
)

BOOST_FUSION_ADAPT_STRUCT(
  dbc::message_base,
  (std::uint32_t, id)
  (std::int32_t, cycle_time)
  (std::string, name)
  (std::uint8_t, dlc)
)


#endif  // DBC_PARSER_H
