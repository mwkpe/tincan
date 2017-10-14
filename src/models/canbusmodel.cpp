#include "canbusmodel.h"


#include <memory>

#include "tincan/bussignal.h"
#include "tincan/canframe.h"
#include "tincan/canbus.h"
#include "treeitem.h"
#include "bussignalitem.h"
#include "canframeitem.h"


tin::Can_bus_model::Can_bus_model(const Can_bus* can_bus, QObject* parent)
    : Tree_model{parent}, can_bus_{can_bus}
{
  construct();
}


void tin::Can_bus_model::construct()
{
  root_item_ = std::make_unique<Tree_item>();
  column_headers_ = {"Object", "ID / Phys", "Time / Raw", "Cycle", "Data"};
}


void tin::Can_bus_model::reset()
{
  beginResetModel();
  frame_items_.clear();
  root_item_ = std::make_unique<Tree_item>();
  endResetModel();
}


void tin::Can_bus_model::update_data(std::uint32_t frame_id)
{
  if (auto* item = find_item(frame_id); item) {  // Update existing frame
    item->update();
    auto row = item->row();
    if (row >= 0)
      dataChanged(index(row, 0), index(row, columnCount()));
  }
  else {  // Add new frame
    if (auto* frame = can_bus_->frame(frame_id); frame) {
      auto frame_item = std::make_unique<Can_frame_item>(frame, root_item_.get());
      for (const auto& signal : frame->bus_signals)
        frame_item->add_child(std::make_unique<Bus_signal_item>(&signal, frame_item.get()));
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
