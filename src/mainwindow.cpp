#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <iostream>

#include <QFileDialog>

#include "util.h"
#include "canframe.h"
#include "dbcparser.h"


Main_window::Main_window(QWidget* parent)
    : QMainWindow{parent}, ui{new Ui::MainWindow}, frame_model_{&can_bus_, &dbc_file_}
{
  ui->setupUi(this);
  setWindowTitle("tincan");

  ui->treeViewFrames->setModel(&frame_model_);

  connect(ui->pushOpenClose, &QPushButton::clicked, this, [this] {
    if (can_receiver_.is_open()) {
      can_receiver_.close();
      ui->pushOpenClose->setText("Open");
      ui->pushLoadDbc->setEnabled(true);
    }
    else {
      if (can_receiver_.open(ui->lineIp->text(), ui->linePort->text().toUShort())) {
        ui->pushOpenClose->setText("Close");
        ui->pushLoadDbc->setEnabled(false);
      }
    }
  });

  connect(&can_receiver_, &can::Receiver::received_frame, &can_bus_, &can::Bus::add_frame);
  connect(&can_receiver_, &can::Receiver::received_frame_id,
      &frame_model_, &tin::Frame_model::update_frame);

  connect(ui->pushLoadDbc, &QPushButton::clicked, this, [this] {
    auto filename = QFileDialog::getOpenFileName(this, tr("Open DBC file"), QString{},
      tr("DBC (*.dbc)"));
    try {
      util::Timer timer{true};
      dbc_file_ = dbc::parse(filename.toStdString());
      std::cout << dbc_file_.frame_defs.size() << '\n' << timer.stop_seconds() << std::endl;
    }
    catch (const dbc::Parse_error& e) {
      std::cerr << e.what() << std::endl;
    }
  });
}


Main_window::~Main_window()
{
  delete ui;
}
