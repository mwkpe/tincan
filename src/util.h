#ifndef UTIL_H
#define UTIL_H


#include <chrono>


namespace util
{


class timer
{
public:
  timer() = default;
  timer(bool start_running);
  ~timer() = default;

  void start();
  void stop();
  float stop_seconds();
  float seconds();

private:
  std::chrono::high_resolution_clock::time_point start_time;
  std::chrono::high_resolution_clock::time_point stop_time;
};


}  // namespace util


#endif  // UTIL_H
