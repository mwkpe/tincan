#ifndef NETWORK_UDP_SOCKETGUARD_H
#define NETWORK_UDP_SOCKETGUARD_H


/* RAII wrapper for an ASIO UDP socket */


#include <asio.hpp>


namespace network::udp {


class Socket_guard final
{
public:
  explicit Socket_guard(asio::ip::udp::socket& s) : socket_{s} { }
  ~Socket_guard() { if (socket_.is_open() && !is_released_) socket_.close(); }
  Socket_guard(const Socket_guard&) = delete;
  Socket_guard& operator=(const Socket_guard&) = delete;
  void release() { is_released_ = true; }
  bool has_open_socket() const { return socket_.is_open(); }

private:
  bool is_released_ = false;
  asio::ip::udp::socket& socket_;
};


}  // namespace network::udp


#endif  // NETWORK_UDP_SOCKETGUARD_H
