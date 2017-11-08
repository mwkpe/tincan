#ifndef TIN_CANBUSDEFMODEL_H
#define TIN_CANBUSDEFMODEL_H


#include <cstdint>
#include <vector>
#include <unordered_map>

#include <QVariant>
#include <QModelIndex>

#include "treemodel.h"


namespace tin
{


class Can_bus_def;


class Can_bus_def_model final : public Tree_model
{
  Q_OBJECT

public:
  explicit Can_bus_def_model(QObject* parent = nullptr);
  virtual void construct() override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  void set(const Can_bus_def* bus_def);
  void set_row_height(int h) { row_height_ = h; }

public slots:
  void reset();

private:
  std::vector<int> column_widths_;
  int row_height_ = 20;
};


}  // namespace tin


#endif  // TIN_CANBUSDEFMODEL_H
