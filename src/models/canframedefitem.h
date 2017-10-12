#ifndef TIN_FRAMEDEFITEM_H
#define TIN_FRAMEDEFITEM_H


#include <QVariant>

#include "tincan/canframedef.h"
#include "treeitem.h"


namespace tin
{


class Can_frame_def_item final : public Tree_item
{
public:
  Can_frame_def_item(const Can_frame_def* can_frame_def, Tree_item* parent = nullptr)
      : Tree_item{parent}, can_frame_def_{can_frame_def} {}
  virtual QVariant data(int column) const override;

private:
  const Can_frame_def* can_frame_def_;
};


}  // namespace tin


#endif  // TIN_FRAMEDEFITEM_H
