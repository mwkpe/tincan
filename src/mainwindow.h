#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <QMainWindow>

#include "canreceiver.h"
#include "canbus.h"
#include "dbcfile.h"
#include "framemodel.h"


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
  can::Bus can_bus_;
  dbc::File dbc_file_;
  tin::Frame_model frame_model_;
};


#endif  // MAIN_WINDOW_H
