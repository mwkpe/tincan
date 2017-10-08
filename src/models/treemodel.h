#ifndef TIN_TREEMODEL_H
#define TIN_TREEMODEL_H


#include <memory>
#include <vector>

#include <QVariant>
#include <QModelIndex>
#include <QAbstractItemModel>

#include "treeitem.h"


namespace tin
{


class Tree_model : public QAbstractItemModel
{
  Q_OBJECT

public:
  explicit Tree_model(QObject* parent = nullptr) : QAbstractItemModel{parent} {}
  virtual ~Tree_model() {}
  virtual void construct() = 0;

  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex{}) const override;
  QModelIndex parent(const QModelIndex& index) const override;

  int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
  int columnCount(const QModelIndex& parent = QModelIndex{}) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

protected:
  std::unique_ptr<Tree_item> root_item_;
  std::vector<QString> column_headers_;
};


}  // namespace tin


#endif  // TIN_TREEMODEL_H
