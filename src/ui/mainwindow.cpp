#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <iostream>
#include <chrono>
#include <thread>

#include <QFileDialog>
#include <QChart>
#include <QLineSeries>
#include <QPainter>
#include <QFont>
#include <QGraphicsLayout>
#include <QMargins>

#include "util.h"
#include "network/canrawframe.h"
#include "file/dbcparser.h"
#include "tincan/errors.h"
#include "tincan/translate.h"
#include "tincan/busdefreader.h"
#include "tincan/busdefwriter.h"


Q_DECLARE_METATYPE(can::Raw_frame)


Main_window::Main_window(QWidget* parent)
    : QMainWindow{parent}, ui{new Ui::MainWindow}, can_bus_model_{&can_bus_}
{
  ui->setupUi(this);
  setWindowTitle("tincan");

  qRegisterMetaType<can::Raw_frame>("Raw_frame");

  // Qt Designer bug keeps setting this false
  ui->treeViewFrames->header()->setVisible(true);

  ui->treeViewFrames->setModel(&can_bus_model_);
  ui->treeViewBusDef->setModel(&can_bus_def_model_);
  ui->treeViewFrames->setAlternatingRowColors(true);
  ui->treeViewBusDef->setAlternatingRowColors(true);
  ui->treeViewBusDef->setFont(QFont{"Consolas"});
  ui->treeViewFrames->setFont(QFont{"Consolas"});

  connect(ui->pushOpenClose, &QPushButton::clicked, this, [this] {
    if (can_receiver_.is_running()) {
      can_receiver_.stop();
      can_bus_.reset_frames();
      can_bus_model_.reset();
      ui->pushOpenClose->setText("Open");
    }
    else {
      std::thread{&can::Receiver::start, &can_receiver_, ui->lineIp->text().toStdString(),
          ui->linePort->text().toUShort()}.detach();
      ui->pushOpenClose->setText("Close");
    }
  });

  connect(&can_receiver_, &can::Receiver::received_frame,
      &can_bus_, &tin::Can_bus::add_frame, Qt::QueuedConnection);
  connect(&can_receiver_, &can::Receiver::received_frame_id,
      &can_bus_model_, &tin::Can_bus_model::update_data, Qt::QueuedConnection);

  connect(ui->pushSaveAsBusDef, &QPushButton::clicked, this, [this]{
    auto filepath = QFileDialog::getSaveFileName(this, tr("Save bus definition"), QString{},
      tr("JSON (*.json)"));
    try {
      tin::write_can_bus_def(can_bus_def_, filepath.toStdString());
    } catch (const tin::File_error& e) {
      std::cerr << e.what() << std::endl;
    }
  });

  connect(ui->pushOpenBusDef, &QPushButton::clicked, this, [this]{
    auto filepath = QFileDialog::getOpenFileName(this, tr("Open bus definition"), QString{},
      tr("JSON (*.json)"));
    try {
      util::Timer timer{true};
      can_bus_def_ = tin::read_can_bus_def(filepath.toStdString());
      std::cout << timer.stop_seconds() << std::endl;
      can_bus_.set_definition(&can_bus_def_);
      can_bus_def_model_.set(&can_bus_def_);
      ui->lineBusDefFile->setText(QString::fromStdString(can_bus_def_.source_name));
    }
    catch (const tin::File_error& e) {
      std::cerr << e.what() << std::endl;
    }
  });

  connect(ui->pushImportBusDef, &QPushButton::clicked, this, [this]{
    auto filepath = QFileDialog::getOpenFileName(this, tr("Import bus description file"), QString{},
      tr("DBC (*.dbc)"));
    try {
      util::Timer timer{true};
      dbc_file_ = dbc::parse(filepath.toStdString());
      std::cout << dbc_file_.frame_defs.size() << '\n' << timer.stop_seconds() << std::endl;
      can_bus_def_ = tin::translate(dbc_file_);
      can_bus_.set_definition(&can_bus_def_);
      can_bus_def_model_.set(&can_bus_def_);
      ui->lineBusDefFile->setText(QString::fromStdString(can_bus_def_.source_name));
    }
    catch (const dbc::Parse_error& e) {
      std::cerr << e.what() << std::endl;
    }
  });
  connect(ui->pushClearBusDef, &QPushButton::clicked,
      &can_bus_def_model_, &tin::Can_bus_def_model::reset);
  connect(ui->pushClearBusDef, &QPushButton::clicked, ui->lineBusDefFile, &QLineEdit::clear);

  connect(ui->pushClearBusFrames, &QPushButton::clicked,
      &can_bus_model_, &tin::Can_bus_model::reset);

  chart_view_.setChart(new QtCharts::QChart{});  // QChartView takes ownership
  //chart_view_.setRenderHint(QPainter::Antialiasing, true);
  QtCharts::QChart* chart = chart_view_.chart();
  chart->layout()->setContentsMargins(0, 0, 0, 0);
  chart->setBackgroundRoundness(0);
  chart->setTheme(QtCharts::QChart::ChartTheme::ChartThemeDark);
  //chart->setTitle("CRC");
  chart->setMargins(QMargins{4, 4, 4, 4});
  QtCharts::QLineSeries* series = new QtCharts::QLineSeries{};
  series->setUseOpenGL();
  series->setName("CRC");
  series->setColor(QColor{51,231,247});
//  std::uint8_t val = 0;
//  for (int i=0; i<512; i++)
//    series->append(i, val++);
  series->append(0, 0);
  series->append(255, 255);
  series->append(256, 0);
  series->append(511, 255);
  chart->addSeries(series);
  //chart->setBackgroundVisible(false);
  chart->setBackgroundBrush(QBrush{QColor{25, 25, 25}});
  chart->createDefaultAxes();
  chart->axisX()->setGridLineVisible(false);
  chart->axisY()->setGridLineVisible(false);
  chart->axisX()->setMax(128);
  chart->axisY()->setMax(64);
  ui->verticalChartLayout->addWidget(&chart_view_);

  ui->splitter->setStretchFactor(0, 3);
  ui->splitter->setStretchFactor(1, 1);
}


Main_window::~Main_window()
{
  using namespace std::chrono_literals;

  // Stop receiver
  if (can_receiver_.is_running()) {
    can_receiver_.stop();
    std::this_thread::sleep_for(100ms);
  }

  delete ui;
}
