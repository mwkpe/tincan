#ifndef TIN_SIGNALITEM_H
#define TIN_SIGNALITEM_H


#include <QVariant>

#include "tincan/bussignal.h"
#include "treeitem.h"


namespace tin
{


class Bus_signal_item final : public Tree_item
{
public:
  Bus_signal_item(const Bus_signal* bus_signal_, Tree_item* parent = nullptr)
      : Tree_item{parent}, bus_signal_{bus_signal_} {}
  virtual QVariant data(int column) const override;

private:
  const Bus_signal* bus_signal_;
};


}  // namespace tin


#endif  // TIN_SIGNALITEM_H
