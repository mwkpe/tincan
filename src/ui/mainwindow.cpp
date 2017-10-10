#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <iostream>

#include <QFileDialog>

#include "util.h"
#include "network/canrawframe.h"
#include "file/dbcparser.h"
#include "tincan/translate.h"


Main_window::Main_window(QWidget* parent)
    : QMainWindow{parent}, ui{new Ui::MainWindow}, can_bus_model_{&can_bus_}
{
  ui->setupUi(this);
  setWindowTitle("tincan");

  ui->treeViewFrames->setModel(&can_bus_model_);

  connect(ui->pushOpenClose, &QPushButton::clicked, this, [this] {
    if (can_receiver_.is_open()) {
      can_receiver_.close();
      can_bus_.reset_frames();
      can_bus_model_.reset();
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

  connect(&can_receiver_, &can::Receiver::received_frame, &can_bus_, &tin::Can_bus::add_frame);
  connect(&can_receiver_, &can::Receiver::received_frame_id,
      &can_bus_model_, &tin::Can_bus_model::update_data);

  connect(ui->pushLoadDbc, &QPushButton::clicked, this, [this] {
    auto filename = QFileDialog::getOpenFileName(this, tr("Open DBC file"), QString{},
      tr("DBC (*.dbc)"));
    try {
      util::Timer timer{true};
      dbc_file_ = dbc::parse(filename.toStdString());
      std::cout << dbc_file_.frame_defs.size() << '\n' << timer.stop_seconds() << std::endl;
      can_bus_def_ = tin::translate(dbc_file_);
      can_bus_.set_definition(&can_bus_def_);
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
