#include "canbusdefmodel.h"


#include <memory>

#include <QSize>
#include <QColor>
#include <QBrush>

#include "tincan/bussignaldef.h"
#include "tincan/canframedef.h"
#include "tincan/canbusdef.h"
#include "treeitemid.h"
#include "treeitem.h"
#include "bussignaldefitem.h"
#include "canframedefitem.h"


tin::Can_bus_def_model::Can_bus_def_model(QObject* parent) : Tree_model{parent}
{
  construct();
}


void tin::Can_bus_def_model::construct()
{
  root_item_ = std::make_unique<Tree_item>(Item_id::Root);
  column_headers_ = {"Object", "ID/Pos", "Len", "Mux", "Value", "Order", "Factor", "Offset",
      "Min", "Max", "Unit"};
  column_widths_ = { 200, 80, 40, 40, 70, 60, 140, 140, 140, 140, 80 };
}


QVariant tin::Can_bus_def_model::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant{};

  switch (role) {
    case Qt::SizeHintRole: return QSize{column_widths_[index.column()], row_height_};
    case Qt::ForegroundRole: {
      auto* item = static_cast<Tree_item*>(index.internalPointer());
      auto id = item->id();
      if (id == Item_id::Can_frame_def)
        return QBrush{QColor{"#33E7F7"}};
      return Tree_model::data(index, role);
    }
  }

  return Tree_model::data(index, role);
}


void tin::Can_bus_def_model::reset()
{
  beginResetModel();
  root_item_ = std::make_unique<Tree_item>();
  endResetModel();
}


void tin::Can_bus_def_model::set(const Can_bus_def* bus_def)
{
  if (!bus_def)
    return;

  beginResetModel();
  root_item_ = std::make_unique<Tree_item>();
  for (const auto& frame_def : bus_def->frame_defs) {
    auto frame_def_item = std::make_unique<Can_frame_def_item>(&frame_def, root_item_.get());
    for (const auto& signal_def : frame_def.bus_signal_defs) {
      frame_def_item->add_child(std::make_unique<Bus_signal_def_item>(&signal_def,
          frame_def_item.get()));
    }
    root_item_->add_child(std::move(frame_def_item));
  }
  endResetModel();
}
