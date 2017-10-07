#ifndef TIN_SIGNALITEM_H
#define TIN_SIGNALITEM_H


#include <QVariant>
#include "treeitem.h"

namespace dbc { struct Signal_def; }


namespace tin
{


class Signal_item final : public Tree_item
{
public:
  Signal_item(const dbc::Signal_def* signal_def, Tree_item* parent = nullptr)
      : Tree_item{parent}, signal_def_{signal_def} {}
  virtual QVariant data(int column) const override;

private:
  const dbc::Signal_def* signal_def_;
};


}  // namespace tin


#endif  // TIN_SIGNALITEM_H
