#ifndef DBC_FILE_H
#define DBC_FILE_H


#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>


namespace dbc
{


enum class Byte_order { Moto, Intel };
enum class Value_sign { Signed, Unsigned };


struct Signal_meta_data
{
  std::int8_t factor_precision = 7;
  std::int8_t offset_precision = 7;
  std::int8_t minimum_precision = 7;
  std::int8_t maximum_precision = 7;
};

struct Signal_def
{
  std::string name;
  std::int32_t multiplex_value;
  bool multiplex_switch;
  std::uint32_t pos;
  std::uint32_t len;
  Byte_order order;
  Value_sign sign;
  double factor;
  double offset;
  double minimum;
  double maximum;
  std::string unit;
  std::vector<std::string> receiver;
  Signal_meta_data meta_data;
};

struct Frame_def
{
  std::uint32_t id;
  std::string name;
  std::uint32_t dlc;
  std::string transmitter;
  std::vector<Signal_def> signal_defs;
};

struct File
{
  std::string name;
  std::vector<std::string> nodes;
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
