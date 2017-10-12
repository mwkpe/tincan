#ifndef TIN_SIGNALITEM_H
#define TIN_SIGNALITEM_H


#include <QVariant>
#include "treeitem.h"


namespace tin
{


struct Bus_signal;

class Bus_signal_item final : public Tree_item
{
public:
  Bus_signal_item(const Bus_signal* bus_signal, Tree_item* parent = nullptr)
      : Tree_item{parent}, bus_signal_{bus_signal} {}
  virtual QVariant data(int column) const override;

private:
  const Bus_signal* bus_signal_;
};


}  // namespace tin


#endif  // TIN_SIGNALITEM_H
