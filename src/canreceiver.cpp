#include "canreceiver.h"


#include <chrono>
#include <QNetworkDatagram>


void can::Receiver::handle_received_datagram(const QNetworkDatagram* datagram)
{
  using namespace std::chrono;
  if (datagram && datagram->isValid()) {
    auto data = datagram->data();
    if (data.size() == sizeof(can::Frame)) {
      emit received_frame(duration_cast<milliseconds>(high_resolution_clock::now()
          .time_since_epoch()).count(), *reinterpret_cast<can::Frame*>(data.data()));
      emit received_frame_id(reinterpret_cast<can::Frame*>(data.data())->id);
    }
  }
}
