#ifndef DBC_FILE_H
#define DBC_FILE_H


#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>


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
  std::string unit;
};

struct message
{
  std::uint32_t id;
  std::string name;
  std::uint32_t dlc;
  std::vector<signal> signals_;
};

struct file
{
  std::string name;
  std::vector<message> messages;
};


inline auto find_message(const file& f, std::uint32_t id)
{
  return std::find_if(std::begin(f.messages), std::end(f.messages),
      [id](const auto& m){ return m.id == id; });
}

inline auto find_message(const file& f, std::string_view name)
{
  return std::find_if(std::begin(f.messages), std::end(f.messages),
      [name](const auto& m){ return m.name == name; });
}

inline auto find_signal(const message& m, std::string_view name)
{
  return std::find_if(std::begin(m.signals_), std::end(m.signals_),
      [name](const auto& s){ return s.name == name; });
}


}  // namespace dbc


#endif  // DBC_FILE_H
