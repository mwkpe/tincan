#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H


#include <cstdint>
#include <string>

#include <QObject>
#include <QString>
class QNetworkDatagram;
class QUdpSocket;


class UdpReceiver : public QObject
{
  Q_OBJECT

public:
  UdpReceiver() = default;
  UdpReceiver(const UdpReceiver&) = delete;
  UdpReceiver(UdpReceiver&&) = delete;
  UdpReceiver& operator=(const UdpReceiver&) = delete;
  UdpReceiver& operator=(UdpReceiver&&) = delete;

  bool open(const QString& ip, std::uint16_t port);
  void close();
  bool is_open() const;

protected:
  virtual void handle_received_datagram(const QNetworkDatagram* datagram) = 0;

private:
  void process_read();
  QUdpSocket* socket_ = nullptr;
};


#endif  // UDP_RECEIVER_H
