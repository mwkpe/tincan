#include "util.h"


util::Timer::Timer(bool start_running)
{
  if (start_running)
    start();
}


void util::Timer::start()
{
  start_time = std::chrono::high_resolution_clock::now();
}


void util::Timer::stop()
{
  stop_time = std::chrono::high_resolution_clock::now();
}


float util::Timer::stop_seconds()
{
  stop();
  return seconds();
}


float util::Timer::seconds()
{
  return std::chrono::duration<float>(stop_time - start_time).count();
}
