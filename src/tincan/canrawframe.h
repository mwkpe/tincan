#ifndef TIN_CANRAWFRAME_H
#define TIN_CANRAWFRAME_H


#include <cstdint>
#include <array>


namespace tin {


struct Can_raw_frame
{
  std::uint32_t id;
  std::uint8_t dlc;
  alignas(8) std::array<uint8_t, 8> data;
};


}  // namespace tin


#endif  // TIN_CANRAWFRAME_H
