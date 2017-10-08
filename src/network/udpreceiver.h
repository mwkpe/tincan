#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H


#include <cstdint>
#include <string>

#include <QObject>
#include <QString>

class QNetworkDatagram;
class QUdpSocket;


namespace udp
{


class Receiver : public QObject
{
  Q_OBJECT

public:
  Receiver() = default;
  Receiver(const Receiver&) = delete;
  Receiver(Receiver&&) = delete;
  Receiver& operator=(const Receiver&) = delete;
  Receiver& operator=(Receiver&&) = delete;

  bool open(const QString& ip, std::uint16_t port);
  void close();
  bool is_open() const;

protected:
  virtual void handle_received_datagram(const QNetworkDatagram* datagram) = 0;

private:
  void process_read();
  QUdpSocket* socket_ = nullptr;
};


}  // namespace udp


#endif  // UDP_RECEIVER_H
