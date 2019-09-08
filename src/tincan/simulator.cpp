#include "simulator.h"


#include <chrono>
#include <thread>
#include <random>
#include "util.h"


namespace
{


struct Frame_flux
{
  std::uint32_t power_level;
  std::uint32_t dispersal_rate;
};


struct Frame_date_time
{
  std::uint8_t time_type : 2;
  std::uint8_t year_0 : 6;
  std::uint8_t year_1;
  std::uint8_t month : 4;
  std::uint8_t day_0 : 4;
  std::uint8_t day_1 : 1;
  std::uint8_t am_pm : 1;
  std::uint8_t hour : 4;
  std::uint8_t minute_0 : 2;
  std::uint8_t minute_1 : 4;
};


}  // namespace


void tin::Simulator::start()
{
  if (!stop_.load())
    std::thread{&tin::Simulator::run, this}.detach();
}


void tin::Simulator::run()
{
  can::Raw_frame flux;
  flux.id = 233;
  flux.dlc = 6;
  auto* flux_data = reinterpret_cast<Frame_flux*>(flux.data.data());
  flux_data->power_level = 1210000000;
  flux_data->dispersal_rate = 7743;

  can::Raw_frame date_time;
  date_time.id = 245;
  date_time.dlc = 5;
  auto* date_time_data = reinterpret_cast<Frame_date_time*>(date_time.data.data());
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
  date_time_data->year_0 = 1955;
  date_time_data->year_1 = 1955 >> 6;
  date_time_data->month = 11;
  date_time_data->day_0 = 5;
  date_time_data->day_1 = 5 >> 4;
  date_time_data->hour = 6;
  date_time_data->minute_0 = 31;
  date_time_data->minute_1 = 31 >> 2;
#pragma GCC diagnostic pop

  std::mt19937_64 engine(0xE3E00799540DE706);
  std::uniform_int_distribution<std::int32_t> dist{1, 512};

  while (!stop_.load()) {
    using namespace std::chrono;
    {
      util::Timer delta_timer{true};
      std::this_thread::sleep_for(milliseconds{20});
      auto delta = delta_timer.stop_milliseconds();
      accumulator1_ += delta;
      accumulator2_ += delta;
    }

    if (accumulator1_ >= 200) {
      accumulator1_ = 0;
      flux_data->dispersal_rate = static_cast<std::uint32_t>(7743 + dist(engine) - 256);
      auto time = duration_cast<milliseconds>(high_resolution_clock::now()
          .time_since_epoch()).count();
      emit received_frame(static_cast<std::uint64_t>(time), flux);
    }

    if (accumulator2_ >= 1000) {
      accumulator2_ = 0;
      auto time = duration_cast<milliseconds>(high_resolution_clock::now()
          .time_since_epoch()).count();
      emit received_frame(static_cast<std::uint64_t>(time), date_time);
    }
  }

  stop_.store(false);
}
