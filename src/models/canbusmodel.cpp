#include "canbusmodel.h"

#include <iostream>
#include <memory>

#include <QTimer>
#include <QColor>
#include <QBrush>

#include "tincan/canframe.h"
#include "tincan/canbus.h"
#include "models/treeitemid.h"
#include "models/treeitem.h"
#include "models/cansignalitem.h"
#include "models/canframeitem.h"


tin::Can_bus_model::Can_bus_model(const Can_bus* can_bus, QObject* parent)
    : Tree_model{parent}, can_bus_{can_bus}
{
  construct();

  // Deferred update of tree items since multiple frames with low cycle times cause high CPU usage
  // due to an excessive amount of data update calls. Be aware, depening on the refresh rate and
  // a frame's cycle time this may lead to some changes not being displayed
  auto* update_timer = new QTimer{this};

  connect(update_timer, &QTimer::timeout, this, [this]{
    for (auto id : deferred_updates_) {
      update_data(id);
    }

    deferred_updates_.clear();
  });

  update_timer->start(53);
}


void tin::Can_bus_model::construct()
{
  reset();
  column_headers_ = {"Object", "Value", "Time / Unit", "Cycle", "Length", "Data"};
  column_widths_ = {240, 160, 120, 80, 70, 120};
}


QVariant tin::Can_bus_model::data(const QModelIndex& index, int role) const
{
  if (!index.isValid()) {
    return QVariant{};
  }

  switch (role) {
    case Qt::SizeHintRole: {
      return QSize{column_widths_[static_cast<std::size_t>(index.column())], row_height_};
    }
    break;
    case Qt::ForegroundRole: {
      auto* item = reinterpret_cast<Tree_item*>(index.internalPointer());
      if (item->id() == Tree_item_id::Can_frame) {
        const auto* frame_item = reinterpret_cast<const Can_frame_item*>(item);

        if (!frame_item->frame()->alive) {
          return QBrush{QColor{0x787878}};
        }

        if (item->child_count() > 0) {
          return QBrush{QColor{0x33E7F7}};
        }
      }

      return Tree_model::data(index, role);
    }
    break;
    default:;
  }

  return Tree_model::data(index, role);
}


void tin::Can_bus_model::reset()
{
  beginResetModel();
  frame_items_.clear();
  root_item_ = std::make_unique<Tree_item>(Tree_item_id::Root);
  auto empty_item = std::make_unique<Tree_item>(Tree_item_id::Empty, root_item_.get());
  root_item_->add_child(std::move(empty_item));
  endResetModel();

  is_empty_ = true;
}


void tin::Can_bus_model::clear()
{
  beginResetModel();
  frame_items_.clear();
  root_item_ = std::make_unique<Tree_item>(Tree_item_id::Root);
  endResetModel();
}


void tin::Can_bus_model::update_data_deferred(std::uint32_t frame_id)
{
  deferred_updates_.insert(frame_id);
}


void tin::Can_bus_model::update_data(std::uint32_t frame_id)
{
  if (auto* item = find_item(frame_id); item) {  // Update existing frame
    auto row = item->row();
    if (row >= 0) {
      // Update frame
      auto parent_index = index(row, 0);
      emit dataChanged(parent_index, index(row, columnCount() - 1), QVector<int>{Qt::DisplayRole});

      // Update signals
      if (int child_count = rowCount(parent_index); child_count > 0) {
        emit dataChanged(index(0, 0, parent_index), index(child_count - 1,
            columnCount() - 1, parent_index));
      }
    }
  }
  else {  // Add new frame
    if (auto* frame = can_bus_->get_frame(frame_id); frame) {
      if (is_empty_) {
        is_empty_ = false;
        clear();
      }

      auto frame_item = std::make_unique<Can_frame_item>(frame, root_item_.get());
      for (const auto& signal : frame->can_signals) {
        frame_item->add_child(std::make_unique<Can_signal_item>(&signal, frame_item.get()));
      }
      frame_items_.insert({frame_id, frame_item.get()});
      beginInsertRows(QModelIndex{}, rowCount(), rowCount());
      root_item_->add_child(std::move(frame_item));
      endInsertRows();
    }
  }
}


tin::Can_frame_item* tin::Can_bus_model::find_item(std::uint32_t frame_id)
{
  auto it = frame_items_.find(frame_id);

  if (it != std::end(frame_items_)) {
    return it->second;
  }

  return nullptr;
}
