#ifndef TIN_CANFRAMEITEM_H
#define TIN_CANFRAMEITEM_H


#include <QVariant>

#include "models/treeitem.h"
#include "models/treeitemid.h"


namespace tin {


struct Can_frame;


class Can_frame_item final : public Tree_item
{
public:
  Can_frame_item(const Can_frame* can_frame, Tree_item* parent = nullptr)
      : Tree_item{Tree_item_id::Can_frame, parent}, can_frame_{can_frame} {}
  const Can_frame* frame() const { return can_frame_; }
  virtual QVariant data(int column) const override;

private:
  const Can_frame* can_frame_;
};


}  // namespace tin


#endif  // TIN_CANFRAMEITEM_H
