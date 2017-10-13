#ifndef NET_UDP_ASYNCRECEIVER_H
#define NET_UDP_ASYNCRECEIVER_H


/* Base class for implementing an asynchronous UDP receiver. A derived class must implement
   handle_receive which gets passed a view of the received data. The function start() will
   block and should be called in its own thread. */


#include <cstdint>
#include <array>
#include <string>

#include <gsl/gsl>
#include <asio.hpp>


// Telling MOC to skip this since it leads to a random parse error at "net"
#ifndef Q_MOC_RUN
namespace net::udp
{
#endif


class Async_receiver
{
public:
  Async_receiver() : socket_{io_service_} {}
  virtual ~Async_receiver();
  Async_receiver(const Async_receiver&) = delete;
  Async_receiver& operator=(const Async_receiver&) = delete;

  void start(const std::string& ip, std::uint16_t port);
  void stop();
  bool is_running() const { return socket_.is_open(); }

protected:
  virtual void handle_receive(gsl::span<std::uint8_t> buffer) = 0;

private:
  void start_async_receive();

  asio::io_service io_service_;
  asio::ip::udp::socket socket_;
  std::array<std::uint8_t, 128> buffer_;
};


} // namespace net::udp


#endif  // NET_UDP_ASYNCRECEIVER_H
