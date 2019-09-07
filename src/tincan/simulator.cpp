#include "simulator.h"


#include <chrono>
#include <thread>
#include "util.h"


void tin::Simulator::start()
{
  if (!stop_.load())
    std::thread{&tin::Simulator::run, this}.detach();
}


void tin::Simulator::run()
{
  using namespace std::chrono;

  can::Raw_frame frame;
  frame.id = 42;
  frame.dlc = 8;

  while (!stop_.load()) {
    {
      util::Timer timer{true};
      std::this_thread::sleep_for(milliseconds{20});
      accumulator_ += timer.stop_milliseconds();
    }

    if (accumulator_ >= 200) {
      accumulator_ = 0;
      auto time = duration_cast<milliseconds>(high_resolution_clock::now()
          .time_since_epoch()).count();
      frame.data[0]++;
      emit received_frame(static_cast<std::uint64_t>(time), frame);
    }
  }

  stop_.store(false);
}
