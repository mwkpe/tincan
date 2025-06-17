#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <vector>

#include <QMainWindow>
#include <QTimer>

#include "network/canudpreceiver.h"
#include "network/pcanusbtransceiver.h"
#include "tincan/canbus.h"
#include "tincan/simulator.h"
#include "tincan/canbusdef.h"
#include "tincan/cantracer.h"
#include "file/dbcfile.h"
#include "models/canbusmodel.h"
#include "models/canbusdefmodel.h"


namespace Ui { class MainWindow; }


class Main_window : public QMainWindow
{
  Q_OBJECT

public:
  explicit Main_window(QWidget* parent = nullptr);
  ~Main_window();

private:
  void reset();

  Ui::MainWindow* ui;
  QTimer update_timer_;
  network::Can_udp_receiver can_udp_receiver_;
  network::Pcan_usb_transceiver pcan_usb_transceiver_;
  std::vector<network::Pcan_channel> pcan_channels_;
  dbc::File dbc_file_;
  tin::Can_bus can_bus_;
  tin::Simulator simulator_;
  tin::Can_bus_def can_bus_def_;
  tin::Can_bus_model can_bus_model_;
  tin::Can_bus_def_model can_bus_def_model_;
  tin::Can_tracer can_tracer_;
};


#endif  // MAIN_WINDOW_H
