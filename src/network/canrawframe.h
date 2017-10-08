#ifndef CAN_FRAME_H
#define CAN_FRAME_H


#include <cstdint>
#include <array>


namespace can
{


struct Raw_frame
{
  std::uint32_t id;
  std::uint8_t dlc;
  alignas(8) std::array<uint8_t, 8> data;
};


}  // namespace can


#endif  // CAN_FRAME_H
