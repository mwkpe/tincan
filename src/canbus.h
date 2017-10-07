#ifndef CANBUS_H
#define CANBUS_H


#include <cstdint>
#include <tuple>
#include <unordered_map>

#include <QObject>

#include "canframe.h"


namespace can
{


class Bus : public QObject
{
  Q_OBJECT

public:
  std::tuple<std::uint64_t, can::Frame> frame(std::uint32_t id) const;

public slots:
  void add_frame(std::uint64_t time, can::Frame Frame);

private:
  std::unordered_map<std::uint32_t, std::tuple<std::uint64_t, can::Frame>> frames_;
};


}  // namespace can


#endif  // CANBUS_H
