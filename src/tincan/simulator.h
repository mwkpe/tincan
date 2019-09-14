#ifndef TIN_SIMULATOR_H
#define TIN_SIMULATOR_H


#include <cstdint>
#include <atomic>

#include <QObject>

#include "tincan/canrawframe.h"


namespace tin {


class Simulator final : public QObject
{
  Q_OBJECT

public:
  Simulator() = default;
  Simulator(const Simulator&) = delete;
  Simulator(Simulator&&) = delete;
  Simulator& operator=(const Simulator&) = delete;
  Simulator& operator=(Simulator&&) = delete;
  void start();
  void stop() { running_.store(false); }

signals:
  void received_frame(std::uint64_t, tin::Can_raw_frame);

private:
  void run();
  std::atomic<bool> running_ = false;
  std::int64_t accumulator1_ = 0;
  std::int64_t accumulator2_ = 0;
  std::int64_t accumulator3_ = 0;
};


}  // namespace tin


#endif  // TIN_SIMULATOR_H
