#include "treemodel.h"


QVariant tin::Tree_model::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    return column_headers_[section];
  }

  return QVariant{};
}


QModelIndex tin::Tree_model::index(int row, int column, const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  Tree_item* item;
  if (!parent.isValid())
    item = root_item_.get();
  else
    item = static_cast<Tree_item*>(parent.internalPointer());

  item = item->child(row);
  if (item)
    return createIndex(row, column, item);

  return QModelIndex{};
}


QModelIndex tin::Tree_model::parent(const QModelIndex& index) const
{
  if (!index.isValid())
    return QModelIndex{};

  auto* item = static_cast<Tree_item*>(index.internalPointer())->parent();

  if (item == root_item_.get() || item == nullptr)
    return QModelIndex{};

  return createIndex(item->row(), 0, item);
}


int tin::Tree_model::rowCount(const QModelIndex& parent) const
{
  if (parent.column() > 0)
    return 0;

  Tree_item* item;
  if (!parent.isValid())
    item = root_item_.get();
  else
    item = static_cast<Tree_item*>(parent.internalPointer());

  return item->child_count();
}


int tin::Tree_model::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return static_cast<int>(column_headers_.size());
}


QVariant tin::Tree_model::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant{};
  if (role != Qt::DisplayRole)
    return QVariant{};

  auto* item = static_cast<Tree_item*>(index.internalPointer());
  return item->data(index.column());
}
