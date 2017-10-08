#include "udpreceiver.h"


#include <QUdpSocket>
#include <QNetworkDatagram>


bool udp::Receiver::open(const QString& ip, std::uint16_t port)
{
  if (socket_)
    close();

  socket_ = new QUdpSocket{this};

  if (socket_)
  {
    if (socket_->bind(QHostAddress{ip}, port))
    {
      connect(socket_, &QUdpSocket::readyRead, this, &udp::Receiver::process_read);
      return true;
    }
  }

  close();
  return false;
}


void udp::Receiver::close()
{
  socket_->deleteLater();
  socket_ = nullptr;
}


void udp::Receiver::process_read()
{
  while (socket_ && socket_->hasPendingDatagrams())
  {
    auto datagram = socket_->receiveDatagram();
    handle_received_datagram(&datagram);
  }
}


bool udp::Receiver::is_open() const
{
  return socket_ != nullptr;
}
