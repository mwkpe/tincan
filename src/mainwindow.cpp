#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <iostream>

#include <QFileDialog>

#include "canframe.h"
#include "dbcparser.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow{parent}, ui{new Ui::MainWindow}
{
  ui->setupUi(this);
  setWindowTitle("tincan");

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
      ui->pushLoadDbc->setEnabled(true);
    }
    else {
      if (can_receiver_.open(ui->lineIp->text(), ui->linePort->text().toUShort())) {
        data_model_.reset();
        ui->pushOpenClose->setText("Close");
        ui->pushLoadDbc->setEnabled(false);
      }
    }
  });

  connect(&can_receiver_, &can::receiver::received_frame,
      &data_model_, &tin::CanFrameTableModel::add_frame);

  connect(ui->pushLoadDbc, &QPushButton::clicked, this, [this] {
    auto filename = QFileDialog::getOpenFileName(this, tr("Open DBC file"), QString{},
      tr("DBC (*.dbc)"));
    try {
     auto dbc_file = dbc::parse(filename.toStdString());
     std::cout << dbc_file.messages.size() << std::endl;
    }
    catch (const dbc::parse_error& e) {
      std::cerr << e.what() << std::endl;
    }
  });
}


MainWindow::~MainWindow()
{
  delete ui;
}
