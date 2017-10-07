#include "framemodel.h"


#include "treeitem.h"
#include "frameitem.h"
#include "signalitem.h"
#include "canbus.h"
#include "dbcfile.h"


tin::Frame_model::Frame_model(const can::Bus* can_bus, const dbc::File* dbc_file, QObject* parent)
    : Tree_model{parent}, can_bus_{can_bus}, dbc_file_{dbc_file}
{
  construct();
}


void tin::Frame_model::construct()
{
  root_item_ = std::make_unique<Tree_item>();
  column_headers_ = {"Object", "ID", "Cycle", "Data"};
}


void tin::Frame_model::reset()
{
  beginResetModel();
  root_item_ = std::make_unique<Tree_item>();
  endResetModel();
}


void tin::Frame_model::update_frame(std::uint32_t id)
{
  auto t = can_bus_->frame(id);
  auto& frame = std::get<1>(t);

  if (auto* item = find_item(id); item) {  // Update existing frame
    item->update_frame(frame);
    auto row = item->row();
    if (row >= 0)
      dataChanged(index(row, 0), index(row, columnCount()));
  }
  else {  // Add new frame
    const dbc::Frame_def* frame_def = dbc::find_frame(*dbc_file_, id);
    auto frame_item = std::make_unique<Frame_item>(frame, frame_def, root_item_.get());
    if (frame_def) {
      for (const auto& signal_def : frame_def->signal_defs) {
        frame_item->add_child(std::make_unique<Signal_item>(&signal_def, frame_item.get()));
      }
    }
    frame_items_.insert({id, frame_item.get()});
    beginInsertRows(QModelIndex{}, rowCount(), rowCount());
    root_item_->add_child(std::move(frame_item));
    endInsertRows();
  }
}


tin::Frame_item* tin::Frame_model::find_item(std::uint32_t frame_id)
{
  auto it = frame_items_.find(frame_id);
  if (it != std::end(frame_items_))
    return it->second;
  return nullptr;
}
