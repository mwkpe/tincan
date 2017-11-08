#ifndef TIN_CANBUSDEF_H
#define TIN_CANBUSDEF_H


#include <string>
#include <vector>
#include <algorithm>

#include "canframedef.h"


namespace tin
{


struct Can_bus_def
{
  std::string source_name;
  std::vector<std::string> nodes;
  std::vector<Can_frame_def> frame_defs;
};


inline const Can_frame_def* find_frame_def(const Can_bus_def& bus_def, std::uint32_t id)
{
  auto it = std::find_if(std::begin(bus_def.frame_defs), std::end(bus_def.frame_defs),
      [id](const auto& fd){ return fd.id == id; });
  if (it != std::end(bus_def.frame_defs))
    return &*it;
  return nullptr;
}

inline const Can_frame_def* find_frame_def(const Can_bus_def& bus_def, std::string_view name)
{
  auto it = std::find_if(std::begin(bus_def.frame_defs), std::end(bus_def.frame_defs),
      [name](const auto& fd){ return fd.name == name; });
  if (it != std::end(bus_def.frame_defs))
    return &*it;
  return nullptr;
}


}  // namespace tin


#endif  // TIN_CANBUSDEF_H
