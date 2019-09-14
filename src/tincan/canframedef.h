#ifndef TIN_CANFRAMEDEF_H
#define TIN_CANFRAMEDEF_H


#include <cstdint>
#include <string>
#include <vector>

#include "tincan/cansignaldef.h"


namespace tin {


struct Can_frame_def
{
  std::uint32_t id;
  std::string name;
  std::uint32_t dlc;
  std::string transmitter;
  bool multiplexer;
  std::vector<Can_signal_def> can_signal_defs;
};


}  // namespace tin


#endif  // TIN_CANFRAMEDEF_H
