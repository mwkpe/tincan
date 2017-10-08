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


struct Signal_def
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

struct Frame_def
{
  std::uint32_t id;
  std::string name;
  std::uint32_t dlc;
  std::vector<Signal_def> signal_defs;
};

struct File
{
  std::string name;
  std::vector<Frame_def> frame_defs;
};


inline const Frame_def* find_frame_def(const File& f, std::uint32_t id)
{
  auto it = std::find_if(std::begin(f.frame_defs), std::end(f.frame_defs),
      [id](const auto& m){ return m.id == id; });
  if (it != std::end(f.frame_defs))
    return &*it;
  return nullptr;
}

inline const Frame_def* find_frame_def(const File& f, std::string_view name)
{
  auto it = std::find_if(std::begin(f.frame_defs), std::end(f.frame_defs),
      [name](const auto& m){ return m.name == name; });
  if (it != std::end(f.frame_defs))
    return &*it;
  return nullptr;
}

inline const Signal_def* find_signal_def(const Frame_def& m, std::string_view name)
{
  auto it = std::find_if(std::begin(m.signal_defs), std::end(m.signal_defs),
      [name](const auto& s){ return s.name == name; });
  if (it != std::end(m.signal_defs))
    return &*it;
  return nullptr;
}


}  // namespace dbc


#endif  // DBC_FILE_H
