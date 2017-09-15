#ifndef TIN_CAN_FRAME_H
#define TIN_CAN_FRAME_H


#include <cstdint>


namespace tin
{


struct CanFrame
{
  std::uint32_t id;
  std::uint8_t dlc;
  alignas(8) std::uint8_t data[8];
};


}  // namespace tin


#endif  // TIN_CAN_FRAME_H
