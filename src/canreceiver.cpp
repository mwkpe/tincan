#include "canreceiver.h"


#include <chrono>
#include <QNetworkDatagram>


void can::receiver::handle_received_datagram(const QNetworkDatagram* datagram)
{
  using namespace std::chrono;
  if (datagram && datagram->isValid()) {
    auto data = datagram->data();
    if (data.size() == sizeof(can::frame)) {
      emit received_frame(duration_cast<milliseconds>(high_resolution_clock::now()
          .time_since_epoch()).count(), *reinterpret_cast<can::frame*>(data.data()));
    }
  }
}
