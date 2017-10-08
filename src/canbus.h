#ifndef TIN_CANBUS_H
#define TIN_CANBUS_H


#include <cstdint>
#include <tuple>
#include <unordered_map>

#include <QObject>

#include "canframe.h"
#include "network/canrawframe.h"


namespace dbc { struct File; }


namespace tin
{


class Can_bus : public QObject
{
  Q_OBJECT

public:
  void set_description(const dbc::File* file) { dbc_file_ = file; };
  void reset_frames() { frames_.clear(); }
  std::tuple<bool, tin::Can_frame> frame(std::uint32_t id) const;

public slots:
  void add_frame(std::uint64_t time, can::Raw_frame bus_frame);

private:
  const dbc::File* dbc_file_ = nullptr;
  std::unordered_map<std::uint32_t, Can_frame> frames_;
};


}  // namespace tin


#endif  // TIN_CANBUS_H
