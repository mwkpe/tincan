#ifndef TIN_CANSIGNALITEM_H
#define TIN_CANSIGNALITEM_H


#include <QVariant>
#include "models/treeitem.h"
#include "models/treeitemid.h"


namespace tin {


struct Can_signal;


class Can_signal_item final : public Tree_item
{
public:
  Can_signal_item(const Can_signal* can_signal, Tree_item* parent = nullptr)
      : Tree_item{Tree_item_id::Can_signal, parent}, can_signal_{can_signal} {}
  const Can_signal* signal() const { return can_signal_; }
  virtual QVariant data(int column) const override;

private:
  const Can_signal* can_signal_;
};


}  // namespace tin


#endif  // TIN_CANSIGNALITEM_H
