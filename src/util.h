#ifndef UTIL_H
#define UTIL_H


#include <cstdint>
#include <chrono>
#include <numeric>
#include <iterator>
#include <array>
#include <algorithm>


namespace util {


class Timer
{
public:
  Timer() = default;
  explicit Timer(bool start_running);
  ~Timer() = default;

  template<typename T = std::int64_t> static T system_now();
  template<typename T = std::int64_t> static T high_res_now();
  void start();
  void stop();
  float stop_seconds();
  float seconds();
  std::int64_t stop_milliseconds();
  std::int64_t milliseconds();

private:
  std::chrono::high_resolution_clock::time_point start_time;
  std::chrono::high_resolution_clock::time_point stop_time;
};


namespace math {


template<typename T> int mean(T collection)
{
  if (std::size(collection) == 0)
    return 0;
  return std::accumulate(std::begin(collection), std::end(collection), 0) / std::size(collection);
}


}  // namespace math


}  // namespace util


#endif  // UTIL_H
