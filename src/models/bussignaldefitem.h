#ifndef TIN_SIGNALDEFITEM_H
#define TIN_SIGNALDEFITEM_H


#include <QVariant>
#include "treeitem.h"


namespace tin
{


struct Bus_signal_def;


class Bus_signal_def_item final : public Tree_item
{
public:
  Bus_signal_def_item(const Bus_signal_def* bus_signal_def, Tree_item* parent = nullptr)
      : Tree_item{parent}, bus_signal_def_{bus_signal_def} {}
  virtual QVariant data(int column) const override;

private:
  const Bus_signal_def* bus_signal_def_;
};


}  // namespace tin


#endif  // TIN_SIGNALDEFITEM_H
