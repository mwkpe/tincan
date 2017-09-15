#include "udpreceiver.h"


#include <QUdpSocket>
#include <QNetworkDatagram>


bool udp::receiver::open(const QString& ip, std::uint16_t port)
{
  if (socket_)
    close();

  socket_ = new QUdpSocket{this};

  if (socket_)
  {
    if (socket_->bind(QHostAddress{ip}, port))
    {
      connect(socket_, &QUdpSocket::readyRead, this, &udp::receiver::process_read);
      return true;
    }
  }

  close();
  return false;
}


void udp::receiver::close()
{
  socket_->deleteLater();
  socket_ = nullptr;
}


void udp::receiver::process_read()
{
  while (socket_ && socket_->hasPendingDatagrams())
  {
    auto datagram = socket_->receiveDatagram();
    handle_received_datagram(&datagram);
  }
}


bool udp::receiver::is_open() const
{
  return socket_ != nullptr;
}
