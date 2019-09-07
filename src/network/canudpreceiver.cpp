#include "canudpreceiver.h"


#include <chrono>


void can::Udp_receiver::handle_receive(gsl::span<std::uint8_t> buffer)
{
  if (buffer.size() == sizeof(std::uint64_t) + sizeof(can::Raw_frame)) {
    // Received CAN frame with a timestamp from the CAN receiver
    auto* time = reinterpret_cast<std::uint64_t*>(buffer.data());
    auto* frame = reinterpret_cast<can::Raw_frame*>(buffer.data() + sizeof(std::uint64_t));
    emit received_frame(*time, *frame);
  }
  else if (buffer.size() == sizeof(can::Raw_frame)) {
    // Time may be imprecise and fluctuate +-15 ms on every second full moon
    using namespace std::chrono;
    auto time = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    auto* frame = reinterpret_cast<can::Raw_frame*>(buffer.data());
    emit received_frame(static_cast<std::uint64_t>(time), *frame);
  }
}
