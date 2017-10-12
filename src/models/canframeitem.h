#ifndef TIN_FRAMEITEM_H
#define TIN_FRAMEITEM_H


#include <QVariant>
#include "treeitem.h"


namespace tin
{


struct Can_frame;


class Can_frame_item final : public Tree_item
{
public:
  Can_frame_item(const Can_frame* can_frame, Tree_item* parent = nullptr)
      : Tree_item{parent}, can_frame_{can_frame} {}
  virtual QVariant data(int column) const override;

private:
  const Can_frame* can_frame_;
};


}  // namespace tin


#endif  // TIN_FRAMEITEM_H
