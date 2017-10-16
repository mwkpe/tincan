#ifndef TIN_TRACER_H
#define TIN_TRACER_H


#include <cstdint>
#include <string>
#include <QObject>
#include "canframe.h"


namespace tin
{


class Can_tracer : public QObject
{
  Q_OBJECT

public:
  void set_frame(const Can_frame* trace_frame) { trace_frame_ = trace_frame;
      prev_frame_.receive_time = 0; }

signals:
  void line_ready(const QString&);

public slots:
  void update_data(std::uint32_t id);

private:
  const Can_frame* trace_frame_ = nullptr;
  Can_frame prev_frame_;
};


}  // namespace tin


#endif  // TIN_TRACER_H
