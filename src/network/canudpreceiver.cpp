#include "canudpreceiver.h"


#include "util.h"


void network::Can_udp_receiver::handle_receive(gsl::span<std::uint8_t> buffer)
{
  if (buffer.size() == sizeof(std::uint64_t) + sizeof(tin::Can_raw_frame)) {
    // Received CAN frame with a timestamp from the CAN receiver
    auto* time = reinterpret_cast<std::uint64_t*>(buffer.data());
    auto* frame = reinterpret_cast<tin::Can_raw_frame*>(buffer.data() + sizeof(std::uint64_t));
    emit received_frame(*time, *frame);
  }
  else if (buffer.size() == sizeof(tin::Can_raw_frame)) {
    // Time may be imprecise and fluctuate +-15 ms on every second full moon
    auto time = util::Timer::high_res_now<std::uint64_t>();
    auto* frame = reinterpret_cast<tin::Can_raw_frame*>(buffer.data());
    emit received_frame(time, *frame);
  }
}
