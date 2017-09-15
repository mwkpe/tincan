#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H


#include <QMainWindow>

#include "canreceiver.h"
#include "canframetablemodel.h"


namespace Ui { class MainWindow; }


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow* ui;
  can::receiver can_receiver_;
  tin::CanFrameTableModel data_model_;
};


#endif  // MAIN_WINDOW_H
