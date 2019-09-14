#ifndef NETWORK_DUMMYRECEIVER_H
#define NETWORK_DUMMYRECEIVER_H


#include <cstdint>
#include <thread>

#include <QObject>

#include "tincan/canrawframe.h"
#include "util.h"


namespace network {


// 1. Add receiver as Main_window class member

// 2. Connect receive signal to Can_bus slot
// connect(&dummy_receiver_, &network::Dummy_receiver::received_frame,
//    &can_bus_, &tin::Can_bus::add_frame, Qt::QueuedConnection);

// 3. Call start() on button press or startup


class Dummy_receiver final : public QObject
{
  Q_OBJECT

public:
  void start()
  {
    if (!running_.load())
      std::thread{&network::Dummy_receiver::run, this}.detach();
  }

  void run()
  {
    running_.store(true);

    // Setup and start actual CAN receiver/interface
    // ...

    while (running_.load()) {
      // Poll, wait, etc. for data
      // ...

      // Copy or reinterpret data to tin::Can_raw_frame
      tin::Can_raw_frame frame;

      // Get timestamp (preferably from receiver)
      std::uint64_t time = util::Timer::high_res_now<std::uint64_t>();

      // Emit signal
      emit received_frame(time, frame);
    }
  }

  void stop() {
    running_.store(false);
  }

signals:
  void received_frame(std::uint64_t, tin::Can_raw_frame);

private:
  std::atomic<bool> running_ = false;
};


}  // namespace network


#endif  // NETWORK_DUMMYRECEIVER_H
