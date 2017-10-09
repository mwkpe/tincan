#ifndef TIN_CANFRAMEDEF_H
#define TIN_CANFRAMEDEF_H


#include <cstdint>
#include <string>
#include <vector>

#include "bussignaldef.h"


namespace tin
{


struct Can_frame_def
{
  std::uint32_t id;
  std::string name;
  std::uint32_t dlc;
  std::vector<Bus_signal_def> bus_signal_defs;
};


}  // namespace tin


#endif  // TIN_CANFRAMEDEF_H
