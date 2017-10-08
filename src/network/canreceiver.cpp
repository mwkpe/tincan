#include "canreceiver.h"


#include <chrono>
#include <QNetworkDatagram>


void can::Receiver::handle_received_datagram(const QNetworkDatagram* datagram)
{
  using namespace std::chrono;
  if (datagram && datagram->isValid()) {
    auto data = datagram->data();
    if (data.size() == sizeof(can::Raw_frame)) {
      auto* frame = reinterpret_cast<can::Raw_frame*>(data.data());
      emit received_frame(duration_cast<milliseconds>(high_resolution_clock::now()
          .time_since_epoch()).count(), *frame);
      emit received_frame_id(frame->id);
    }
  }
}
