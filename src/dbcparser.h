#ifndef DBC_PARSER_H
#define DBC_PARSER_H


#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>


namespace dbc
{


enum class byte_order { moto, intel };
enum class value_sign { signed_, unsigned_ };


struct signal
{
  std::string name;
  std::uint32_t pos;
  std::uint32_t len;
  byte_order order;
  value_sign sign;
  double factor;
  double offset;
  double minimum;
  double maximum;
};


struct message_base
{
  std::uint32_t id;
  std::string name;
  std::uint32_t dlc;
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


#endif  // DBC_PARSER_H
