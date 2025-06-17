#include "valuedefmodel.h"


#include <memory>

#include <QSize>

#include "models/treeitemid.h"
#include "models/treeitem.h"
#include "models/valuedefitem.h"


tin::Value_def_model::Value_def_model(QObject* parent) : Tree_model{parent}
{
  construct();
}


void tin::Value_def_model::construct()
{
  root_item_ = std::make_unique<Tree_item>(Tree_item_id::Root);
  column_headers_ = {"Value", "Definition"};
  column_widths_ = { 200, 200 };
}


QVariant tin::Value_def_model::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant{};

  switch (role) {
    case Qt::SizeHintRole:
      return QSize{column_widths_[static_cast<std::size_t>(index.column())], row_height_};
  }

  return Tree_model::data(index, role);
}


void tin::Value_def_model::reset()
{
  beginResetModel();
  root_item_ = std::make_unique<Tree_item>(Tree_item_id::Root);
  endResetModel();
}


void tin::Value_def_model::set(const std::map<std::int32_t, std::string>& value_definitions)
{
  beginResetModel();
  root_item_ = std::make_unique<Tree_item>(Tree_item_id::Root);
  for (const auto& [key, value] : value_definitions) {
    auto valdef_item = std::make_unique<Value_def_item>(key, value, root_item_.get());
    root_item_->add_child(std::move(valdef_item));
  }
  endResetModel();
}
