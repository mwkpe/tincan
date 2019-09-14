#include "simulator.h"


#include <chrono>
#include <thread>
#include <random>
#include <cmath>
#include "util.h"


namespace {


struct Test_frame
{
  std::uint8_t crc;
  std::uint8_t alive : 4;
  std::uint8_t counter_0 : 4;
  std::uint8_t counter_1;
  std::uint8_t sine_0;
  std::uint8_t sine_1 : 3;
  std::uint8_t dummy_0 : 1;
  std::uint8_t light : 1;
  std::uint8_t color : 3;
  std::uint8_t brightness : 7;
  std::uint8_t dummy_1 : 1;
  std::uint16_t altitude;
};


struct Flux
{
  std::uint32_t power_level;
  std::uint32_t dispersal_rate;
};


struct Date_time
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


std::uint8_t calculate_crc(std::uint8_t* data, int length)
{
  std::uint8_t crc = 0;
  std::uint8_t ext;
  std::uint8_t sum;
  for (int i=0; i<length; ++i) {
    ext = *data;
    for (std::uint8_t j=8; j>0; --j) {
      sum = (crc ^ ext) & 0x01;
      crc >>= 1;
      if (sum)
        crc ^= 0x8C;
      ext >>= 1;
    }
    data++;
  }
  return crc;
}


}  // namespace


void tin::Simulator::start()
{
  if (!running_.load())
    std::thread{&tin::Simulator::run, this}.detach();
}


void tin::Simulator::run()
{
  running_.store(true);

  tin::Can_raw_frame test_frame;
  test_frame.id = 100;
  test_frame.dlc = 8;
  auto* test_frame_data = reinterpret_cast<Test_frame*>(test_frame.data.data());
  test_frame_data->light = 1;
  test_frame_data->color = 4;
  test_frame_data->brightness = 75;
  test_frame_data->altitude = 41337;
  std::uint16_t counter = 0;
  std::uint32_t angle = 0;

  tin::Can_raw_frame flux;
  flux.id = 233;
  flux.dlc = 6;
  auto* flux_data = reinterpret_cast<Flux*>(flux.data.data());
  flux_data->power_level = 1210000000;
  flux_data->dispersal_rate = 7743;

  tin::Can_raw_frame date_time;
  date_time.id = 245;
  date_time.dlc = 5;
  auto* date_time_data = reinterpret_cast<Date_time*>(date_time.data.data());
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
  std::uniform_int_distribution<std::int32_t> dist{1, 256};

  while (running_.load()) {
    using namespace std::chrono;
    {
      util::Timer delta_timer{true};
      std::this_thread::sleep_for(milliseconds{20});
      auto delta = delta_timer.stop_milliseconds();
      accumulator1_ += delta;
      accumulator2_ += delta;
      accumulator3_ += delta;
    }

    if (accumulator1_ >= 200) {
      accumulator1_ = 0;
      test_frame_data->alive++;
      counter++;
      test_frame_data->counter_0 = static_cast<std::uint8_t>(counter);
      test_frame_data->counter_1 = static_cast<std::uint8_t>(counter >> 4);
      angle += 5;
      float sine = std::sin(angle * 3.14159f / 180.0f);
      std::uint16_t sine_raw = static_cast<std::uint16_t>((sine + 1.0f) * 1000.0f + 0.5f);
      test_frame_data->sine_0 = static_cast<std::uint8_t>(sine_raw);
      test_frame_data->sine_1 = static_cast<std::uint8_t>(sine_raw >> 8);
      test_frame_data->crc = calculate_crc(test_frame.data.data() + 1, test_frame.dlc - 1);
      auto time = util::Timer::high_res_now<std::uint64_t>();
      emit received_frame(time, test_frame);
    }

    if (accumulator2_ >= 100) {
      accumulator2_ = 0;
      flux_data->dispersal_rate = static_cast<std::uint32_t>(7743 + dist(engine) - 128);
      auto time = util::Timer::high_res_now<std::uint64_t>();
      emit received_frame(time, flux);
    }

    if (accumulator3_ >= 1000) {
      accumulator3_ = 0;
      auto time = util::Timer::high_res_now<std::uint64_t>();
      emit received_frame(time, date_time);
    }
  }
}
