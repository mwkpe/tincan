#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "canframe.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow{parent}, ui{new Ui::MainWindow}
{
  ui->setupUi(this);
  setWindowTitle("TinCAN");

  ui->tableCanData->setModel(&data_model_);
  ui->tableCanData->setAlternatingRowColors(true);

  if (auto* h = ui->tableCanData->verticalHeader(); h) {
    h->setSectionResizeMode(QHeaderView::Fixed);
    h->setDefaultSectionSize(26);
  }
  if (auto* h = ui->tableCanData->horizontalHeader(); h) {
    h->setStretchLastSection(true);
  }

  connect(ui->pushOpenClose, &QPushButton::clicked, this, [this] {
    if (can_receiver_.is_open()) {
      can_receiver_.close();
      ui->pushOpenClose->setText("Open");
    }
    else {
      if (can_receiver_.open(ui->lineIp->text(), ui->linePort->text().toUShort())) {
        data_model_.reset();
        ui->pushOpenClose->setText("Close");
      }
    }
  });

  connect(&can_receiver_, &tin::CanReceiver::received_frame,
      &data_model_, &tin::CanFrameTableModel::add_frame);
}


MainWindow::~MainWindow()
{
  delete ui;
}
