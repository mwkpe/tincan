#ifndef TIN_CANSIGNALDEFITEM_H
#define TIN_CANSIGNALDEFITEM_H


#include <QVariant>
#include "models/treeitem.h"
#include "models/treeitemid.h"


namespace tin {


struct Can_signal_def;


class Can_signal_def_item final : public Tree_item
{
public:
  Can_signal_def_item(const Can_signal_def* can_signal_def, Tree_item* parent = nullptr)
      : Tree_item{Tree_item_id::Can_signal_def, parent}, can_signal_def_{can_signal_def} {}
  const Can_signal_def* signal_def() const { return can_signal_def_; }
  virtual QVariant data(int column) const override;

private:
  const Can_signal_def* can_signal_def_;
};


}  // namespace tin


#endif  // TIN_CANSIGNALDEFITEM_H
