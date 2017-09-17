#include "util.h"


util::timer::timer(bool start_running)
{
  if (start_running)
    start();
}


void util::timer::start()
{
  start_time = std::chrono::high_resolution_clock::now();
}


void util::timer::stop()
{
  stop_time = std::chrono::high_resolution_clock::now();
}


float util::timer::stop_seconds()
{
  stop();
  return seconds();
}


float util::timer::seconds()
{
  return std::chrono::duration<float>(stop_time - start_time).count();
}
