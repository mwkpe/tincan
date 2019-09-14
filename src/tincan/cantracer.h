#ifndef TIN_CANTRACER_H
#define TIN_CANTRACER_H


#include <cstdint>
#include <string>

#include <QObject>

#include "tincan/canframe.h"


namespace tin {


class Can_signal;


class Can_tracer : public QObject
{
  Q_OBJECT

public:
  void set_frame(const Can_frame* trace_frame) { trace_frame_ = trace_frame;
      prev_frame_.receive_time = 0; trace_signal_ = nullptr; }
  void set_signal(const Can_signal* trace_signal, const Can_frame* trace_frame) {
      trace_signal_ = trace_signal; trace_frame_ = trace_frame; prev_frame_.receive_time = 0; }

signals:
  void line_ready(const QString&);

public slots:
  void update_data(std::uint32_t id);
  void set_paused(bool paused) { paused_ = paused; }
  void reset() { set_frame(nullptr); }

private:
  const Can_frame* trace_frame_ = nullptr;
  const Can_signal* trace_signal_ = nullptr;
  Can_frame prev_frame_;
  bool paused_ = false;
};


}  // namespace tin


#endif  // TIN_CANTRACER_H
