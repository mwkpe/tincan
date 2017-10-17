#ifndef TIN_TRACER_H
#define TIN_TRACER_H


#include <cstdint>
#include <string>
#include <QObject>
#include "canframe.h"


namespace tin
{


class Bus_signal;


class Can_tracer : public QObject
{
  Q_OBJECT

public:
  void set_frame(const Can_frame* trace_frame) { trace_frame_ = trace_frame;
      prev_frame_.receive_time = 0; trace_signal_ = nullptr; }
  void set_signal(const Bus_signal* trace_signal, const Can_frame* trace_frame) {
      trace_signal_ = trace_signal; trace_frame_ = trace_frame; prev_frame_.receive_time = 0; }

signals:
  void line_ready(const QString&);

public slots:
  void update_data(std::uint32_t id);
  void set_paused(bool paused) { paused_ = paused; }
  void reset() { set_frame(nullptr); }

private:
  const Can_frame* trace_frame_ = nullptr;
  const Bus_signal* trace_signal_ = nullptr;
  Can_frame prev_frame_;
  bool paused_ = false;
};


}  // namespace tin


#endif  // TIN_TRACER_H
