#include "canbusmodel.h"


#include <memory>

#include <QTimer>
#include <QColor>
#include <QBrush>

#include "tincan/cansignal.h"
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

  // Deferred update of tree items since multiple frames with low cycle times cause a CPU usage
  // spike due to an excessive amount of data update calls. Be aware, depening on the refresh
  // rate and a frame's cycle time this may lead to some changes not being displayed!
  auto* update_timer = new QTimer{this};
  connect(update_timer, &QTimer::timeout, this, [this]{
    for (auto id : deferred_updates_)
      update_data(id);
    deferred_updates_.clear();
  });
  update_timer->start(53);
}


void tin::Can_bus_model::construct()
{
  root_item_ = std::make_unique<Tree_item>(Item_id::Root);
  column_headers_ = {"Object", "ID / Phys", "Time / Unit", "Cycle", "Length", "Data"};
}


QVariant tin::Can_bus_model::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant{};

  switch (role) {
    case Qt::ForegroundRole: {
      auto* item = reinterpret_cast<Tree_item*>(index.internalPointer());
      if (item->id() == Item_id::Can_frame) {
        const auto* frame_item = reinterpret_cast<const Can_frame_item*>(item);
        if (!frame_item->frame()->alive)
          return QBrush{QColor{"#787878"}};
        if (item->child_count() > 0)
          return QBrush{QColor{"#33E7F7"}};
      }
      return Tree_model::data(index, role);
    }
  }

  return Tree_model::data(index, role);
}


void tin::Can_bus_model::reset()
{
  beginResetModel();
  frame_items_.clear();
  root_item_ = std::make_unique<Tree_item>(Item_id::Root);
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
    if (row >= 0)
      dataChanged(index(row, 0), index(row, columnCount()), QVector<int>{Qt::DisplayRole});
  }
  else {  // Add new frame
    if (auto* frame = can_bus_->frame(frame_id); frame) {
      auto frame_item = std::make_unique<Can_frame_item>(frame, root_item_.get());
      for (const auto& signal : frame->can_signals)
        frame_item->add_child(std::make_unique<Can_signal_item>(&signal, frame_item.get()));
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
  if (it != std::end(frame_items_))
    return it->second;
  return nullptr;
}
