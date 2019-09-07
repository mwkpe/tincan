#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <iostream>
#include <chrono>
#include <thread>

#include <pthread.h>

#include <QFileDialog>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QGraphicsLayout>
#include <QMargins>
#include <QMenu>
#include <QAction>

#include "util.h"
#include "network/canrawframe.h"
#include "file/dbcparser.h"
#include "file/dbcwriter.h"
#include "file/jsonreader.h"
#include "file/jsonwriter.h"
#include "tincan/errors.h"
#include "tincan/translate.h"
#include "models/treeitemid.h"
#include "models/bussignalitem.h"
#include "models/canframeitem.h"


Q_DECLARE_METATYPE(can::Raw_frame)


Main_window::Main_window(QWidget* parent)
    : QMainWindow{parent}, ui{new Ui::MainWindow}, can_bus_model_{&can_bus_}
{
  ui->setupUi(this);
  setWindowTitle("Tincan");
  ui->tabWidgetMain->setCurrentIndex(0);
  ui->tabWidgetDetail->setCurrentIndex(0);

  qRegisterMetaType<can::Raw_frame>("Raw_frame");

  // Qt Designer bug keeps setting this false
  ui->treeFrameView->header()->setVisible(true);

  QFont view_font{"Consolas"};
  QFont trace_font{"Consolas", 10};

  // Set default value for size hint
  can_bus_def_model_.set_row_height(QFontMetrics{view_font}.height() - 4);

  ui->treeFrameView->setModel(&can_bus_model_);
  ui->treeViewBusDef->setModel(&can_bus_def_model_);
  ui->treeFrameView->setAlternatingRowColors(true);
  ui->treeViewBusDef->setAlternatingRowColors(true);
  ui->treeViewBusDef->setFont(view_font);
  ui->treeFrameView->setFont(view_font);
  ui->treeViewBusDef->setUniformRowHeights(true);
  ui->treeFrameView->setUniformRowHeights(true);
  ui->treeFrameView->setContextMenuPolicy(Qt::CustomContextMenu);

  ui->plainTrace->setLineWrapMode(QPlainTextEdit::NoWrap);
  ui->plainTrace->setMaximumBlockCount(256);
  ui->plainTrace->setFont(trace_font);
  //ui->plainTrace->setCenterOnScroll(true);

  ui->splitter->setStretchFactor(0, 2);
  ui->splitter->setStretchFactor(1, 1);

  connect(ui->treeFrameView, &QTreeView::customContextMenuRequested, this, [this]{
    auto index = ui->treeFrameView->currentIndex();
    if (!index.isValid())
      return;
    auto* item = static_cast<tin::Tree_item*>(index.internalPointer());

    QMenu menu;
    QAction trace_frame{"Trace frame"};
    QAction trace_signal{"Trace signal"};
    QAction plot_signal{"Plot signal"};
    switch (item->id()) {
      case tin::Item_id::Can_frame: {
        menu.addAction(&trace_frame);
      }
      break;
      case tin::Item_id::Bus_signal: {
        menu.addAction(&trace_signal);
        menu.addAction(&plot_signal);
      }
      break;
      default:
      break;
    }

    QAction* action = menu.exec(QCursor::pos());
    if (action == &trace_frame) {
      can_tracer_.set_frame(static_cast<const tin::Can_frame_item*>(item)->frame());
      std::cout << "Tracing frame" << std::endl;
    }
    else if (action == &trace_signal) {
      can_tracer_.set_signal(static_cast<const tin::Bus_signal_item*>(item)->signal(),
          static_cast<const tin::Can_frame_item*>(item->parent())->frame());
      std::cout << "Tracing signal" << std::endl;
    }
  });

  connect(&can_tracer_, &tin::Can_tracer::line_ready, ui->plainTrace, &QPlainTextEdit::appendHtml);
  connect(ui->pushClearTrace, &QPushButton::clicked, ui->plainTrace, &QPlainTextEdit::clear);
  connect(ui->pushResetTrace, &QPushButton::clicked, &can_tracer_, &tin::Can_tracer::reset);
  connect(ui->pushResetTrace, &QPushButton::clicked, ui->plainTrace, &QPlainTextEdit::clear);
  connect(ui->checkPauseTrace, &QCheckBox::toggled, &can_tracer_, &tin::Can_tracer::set_paused);
  connect(ui->checkPauseTrace, &QCheckBox::toggled, this, [this](bool paused){
      if (paused) ui->plainTrace->appendHtml("Paused"); });

  connect(ui->pushOpenCloseUdpGateway, &QPushButton::clicked, this, [this]{
    if (can_udp_receiver_.is_running()) {
      can_udp_receiver_.stop();
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(50ms);
      ui->pushOpenCloseUdpGateway->setText("Open");
      reset();
    }
    else {
      std::thread{&can::Udp_receiver::start, &can_udp_receiver_, ui->lineIp->text().toStdString(),
          ui->linePort->text().toUShort()}.detach();
      ui->pushOpenCloseUdpGateway->setText("Close");
    }
  });

  connect(ui->pushStartSimulator, &QPushButton::clicked, &simulator_, &tin::Simulator::start);
  connect(ui->pushStopSimulator, &QPushButton::clicked, &simulator_, &tin::Simulator::stop);

  connect(&can_udp_receiver_, &can::Udp_receiver::received_frame,
      &can_bus_, &tin::Can_bus::add_frame, Qt::QueuedConnection);
  connect(&simulator_, &tin::Simulator::received_frame,
      &can_bus_, &tin::Can_bus::add_frame, Qt::QueuedConnection);

  connect(&can_bus_, &tin::Can_bus::data_changed, &can_tracer_, &tin::Can_tracer::update_data);
  connect(&can_bus_, &tin::Can_bus::data_changed, &can_bus_model_,
      &tin::Can_bus_model::update_data_deferred);

  connect(ui->pushSaveAsBusDef, &QPushButton::clicked, this, [this]{
    auto filepath = QFileDialog::getSaveFileName(this, tr("Save bus definition"), QString{},
      tr("DBC (*.dbc)"));
    try {
      dbc::write(tin::to_dbc_file(can_bus_def_), filepath.toStdString());
    } catch (const dbc::Write_error& e) {
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
      can_bus_def_ = tin::to_can_bus_def(dbc_file_);
      can_bus_.set_definition(&can_bus_def_);
      can_bus_def_model_.set(&can_bus_def_);
      for (int i=0; i<can_bus_def_model_.columnCount(); ++i)
        ui->treeViewBusDef->resizeColumnToContents(i);
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
}


Main_window::~Main_window()
{
  // Stop receiver
  if (can_udp_receiver_.is_running()) {
    can_udp_receiver_.stop();
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
  }

  delete ui;
}


void Main_window::reset()
{
  can_bus_.reset_frames();
  can_bus_model_.reset();
  can_tracer_.set_frame(nullptr);
  ui->plainTrace->clear();
}
