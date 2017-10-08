#include "canbusmodel.h"


#include <memory>

#include "../bussignal.h"
#include "../canframe.h"
#include "../canbus.h"
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
  column_headers_ = {"Object", "ID", "Time", "Data"};
}


void tin::Can_bus_model::reset()
{
  beginResetModel();
  frame_items_.clear();
  root_item_ = std::make_unique<Tree_item>();
  endResetModel();
}


void tin::Can_bus_model::update_frame(std::uint32_t id)
{
  if (auto [b, frame] = can_bus_->frame(id); b) {
    if (auto* item = find_item(id); item) {  // Update existing frame
      item->update_frame(frame);
      auto row = item->row();
      if (row >= 0)
        dataChanged(index(row, 0), index(row, columnCount()));
    }
    else {  // Add new frame
      auto frame_item = std::make_unique<Can_frame_item>(frame, root_item_.get());
      if (frame.frame_def) {
        for (const auto& signal_def : frame.frame_def->signal_defs) {
          frame_item->add_child(std::make_unique<Bus_signal_item>(
              Bus_signal{0ull, 0ull, &signal_def}, frame_item.get()));
        }
      }
      frame_items_.insert({id, frame_item.get()});
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
