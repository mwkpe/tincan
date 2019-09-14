#include "udpasyncreceiver.h"


#include <iostream>
#include "network/udpsocketguard.h"


network::udp::Async_receiver::~Async_receiver()
{
  stop();
}


void network::udp::Async_receiver::stop()
{
  // Cancel async handlers
  if (is_running())
    socket_.cancel();
}


void network::udp::Async_receiver::start(const std::string& ip, std::uint16_t port)
{
  if (is_running())
    return;

  Socket_guard socket_guard{socket_};
  try {
    socket_.open(asio::ip::udp::v4());
    asio::ip::udp::resolver resolver{io_service_};
    asio::ip::udp::resolver::query query{asio::ip::udp::v4(), ip.c_str(),
        std::to_string(port).c_str()};
    asio::ip::udp::endpoint ep = *resolver.resolve(query);
    socket_.bind(ep);
    std::cout << "Listening on " << ep.address().to_string() << ":" << ep.port() << std::endl;
  }
  catch (asio::system_error& e) {
    std::cerr << "UDP socket error:\n" << e.what() << std::endl;
    return;
  }

  if (io_service_.stopped())
    io_service_.reset();

  start_async_receive();  // Queue some work for the io_service
  io_service_.run();  // Block until all work is done
  std::cout << "Receiver stopped" << std::endl;
}


void network::udp::Async_receiver::start_async_receive()
{
  // Start async receives until stop() is called
  socket_.async_receive(asio::buffer(buffer_),
      [this](const asio::error_code& error, std::size_t n) {
        if (!error && n > 0) {
          handle_receive(gsl::span<std::uint8_t>{buffer_}.first(n));
        }
        if (!error)
          start_async_receive();  // Prevent io_service from stopping
  });
}
