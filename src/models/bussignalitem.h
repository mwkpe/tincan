#ifndef TIN_BUSSIGNALITEM_H
#define TIN_BUSSIGNALITEM_H


#include <QVariant>
#include "treeitem.h"
#include "treeitemid.h"


namespace tin
{


struct Bus_signal;

class Bus_signal_item final : public Tree_item
{
public:
  Bus_signal_item(const Bus_signal* bus_signal, Tree_item* parent = nullptr)
      : Tree_item{Item_id::Bus_signal, parent}, bus_signal_{bus_signal} {}
  virtual QVariant data(int column) const override;

private:
  const Bus_signal* bus_signal_;
};


}  // namespace tin


#endif  // TIN_BUSSIGNALITEM_H
