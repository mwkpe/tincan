#ifndef CAN_FRAME_H
#define CAN_FRAME_H


#include <cstdint>


namespace can
{


struct Frame
{
  std::uint32_t id;
  std::uint8_t dlc;
  alignas(8) std::uint8_t data[8];
};


}  // namespace can


#endif  // CAN_FRAME_H
