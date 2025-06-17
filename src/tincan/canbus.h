#ifndef TIN_CANBUS_H
#define TIN_CANBUS_H


#include <cstdint>
#include <tuple>
#include <unordered_map>

#include <boost/circular_buffer.hpp>

#include <QObject>

#include "tincan/canframe.h"
#include "tincan/canrawframe.h"


namespace tin {


struct Can_bus_def;


class Can_bus : public QObject
{
  Q_OBJECT

public:
  void set_definition(const Can_bus_def* bus_def) { bus_def_ = bus_def; }
  void reset_frames() { frames_.clear(); }
  const tin::Can_frame* get_frame(std::uint32_t id) const;

signals:
  void frame_received(std::uint32_t);
  void data_changed(std::uint32_t);

public slots:
  void reset();
  void add_frame(std::uint64_t time, tin::Can_raw_frame raw_frame);
  void update_frames();

private:
  const Can_bus_def* bus_def_ = nullptr;
  std::unordered_map<std::uint32_t, Can_frame> frames_;
  std::unordered_map<std::uint32_t, std::uint64_t> prev_frame_time_;
  std::unordered_map<std::uint32_t, boost::circular_buffer<std::int32_t>> cycle_times_;
};


}  // namespace tin


#endif  // TIN_CANBUS_H
