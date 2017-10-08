#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <QMainWindow>

#include "network/canreceiver.h"
#include "canbus.h"
#include "file/dbcfile.h"
#include "models/canbusmodel.h"


namespace Ui { class MainWindow; }


class Main_window : public QMainWindow
{
  Q_OBJECT

public:
  explicit Main_window(QWidget* parent = nullptr);
  ~Main_window();

private:
  Ui::MainWindow* ui;
  can::Receiver can_receiver_;
  dbc::File dbc_file_;
  tin::Can_bus can_bus_;
  tin::Can_bus_model can_bus_model_;
};


#endif  // MAIN_WINDOW_H
