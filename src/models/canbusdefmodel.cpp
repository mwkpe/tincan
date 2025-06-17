#include "canbusdefmodel.h"


#include <memory>

#include <QSize>
#include <QColor>
#include <QBrush>

#include "tincan/cansignaldef.h"
#include "tincan/canframedef.h"
#include "tincan/canbusdef.h"
#include "models/treeitemid.h"
#include "models/treeitem.h"
#include "models/cansignaldefitem.h"
#include "models/canframedefitem.h"


tin::Can_bus_def_model::Can_bus_def_model(QObject* parent) : Tree_model{parent}
{
  construct();
}


void tin::Can_bus_def_model::construct()
{
  root_item_ = std::make_unique<Tree_item>(Tree_item_id::Root);
  column_headers_ = {"Object", "ID / Pos", "Len", "Mux", "Value", "Order", "Factor", "Offset",
      "Min", "Max", "Unit"};
  column_widths_ = { 200, 80, 40, 40, 70, 60, 140, 140, 140, 140, 80 };
}


QVariant tin::Can_bus_def_model::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant{};

  switch (role) {
    case Qt::SizeHintRole: return QSize{column_widths_[static_cast<std::size_t>(index.column())], row_height_};
    case Qt::ForegroundRole: {
      auto* item = static_cast<Tree_item*>(index.internalPointer());
      auto id = item->id();
      if (id == Tree_item_id::Can_frame_def)
        return QBrush{QColor{"#33E7F7"}};
      return Tree_model::data(index, role);
    }
  }

  return Tree_model::data(index, role);
}


void tin::Can_bus_def_model::reset()
{
  beginResetModel();
  root_item_ = std::make_unique<Tree_item>(Tree_item_id::Root);
  endResetModel();
}


void tin::Can_bus_def_model::set(const Can_bus_def* bus_def)
{
  if (!bus_def)
    return;

  beginResetModel();
  root_item_ = std::make_unique<Tree_item>(Tree_item_id::Root);
  for (const auto& frame_def : bus_def->frame_defs) {
    auto frame_def_item = std::make_unique<Can_frame_def_item>(&frame_def, root_item_.get());
    for (const auto& signal_def : frame_def.can_signal_defs) {
      frame_def_item->add_child(std::make_unique<Can_signal_def_item>(&signal_def,
          frame_def_item.get()));
    }
    root_item_->add_child(std::move(frame_def_item));
  }
  endResetModel();
}
