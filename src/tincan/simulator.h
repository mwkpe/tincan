#ifndef TIN_SIMULATOR_H
#define TIN_SIMULATOR_H


#include <cstdint>
#include <atomic>
#include <QObject>
#include "network/canrawframe.h"


namespace tin
{


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
  void stop() { stop_.store(true); }

signals:
  void received_frame(std::uint64_t, can::Raw_frame);

private:
  void run();
  std::atomic<bool> stop_ = false;
  std::int64_t accumulator_ = 0;
};


}  // namespace tin


#endif  // TIN_SIMULATOR_H
