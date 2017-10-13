#include "canreceiver.h"


#include <chrono>


void can::Receiver::handle_receive(gsl::span<std::uint8_t> buffer)
{
  using namespace std::chrono;

  if (buffer.size() == sizeof(can::Raw_frame)) {
    auto* frame = reinterpret_cast<can::Raw_frame*>(buffer.data());
    emit received_frame(duration_cast<milliseconds>(high_resolution_clock::now()
        .time_since_epoch()).count(), *frame);
    emit received_frame_id(frame->id);
  }
}
