#ifndef TIN_CANBUS_H
#define TIN_CANBUS_H


#include <cstdint>
#include <tuple>
#include <unordered_map>

#include <QObject>

#include "canframe.h"
#include "network/canrawframe.h"


namespace tin
{


struct Can_bus_def;


class Can_bus : public QObject
{
  Q_OBJECT

public:
  void set_definition(const Can_bus_def* bus_def) { bus_def_ = bus_def; }
  void reset_frames() { frames_.clear(); }
  const tin::Can_frame* frame(std::uint32_t id) const;

signals:
  void data_changed(std::uint32_t);

public slots:
  void add_frame(std::uint64_t time, can::Raw_frame bus_frame);

private:
  const Can_bus_def* bus_def_ = nullptr;
  std::unordered_map<std::uint32_t, Can_frame> frames_;
  std::unordered_map<std::uint32_t, std::uint64_t> prev_frame_time_;
};


}  // namespace tin


#endif  // TIN_CANBUS_H
