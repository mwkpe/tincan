#ifndef TIN_FRAMEITEM_H
#define TIN_FRAMEITEM_H


#include <QVariant>

#include "tincan/canframe.h"
#include "treeitem.h"


namespace tin
{


class Can_frame_item final : public Tree_item
{
public:
  Can_frame_item(const Can_frame* frame, Tree_item* parent = nullptr)
      : Tree_item{parent}, frame_{frame} {}
  virtual QVariant data(int column) const override;

private:
  const Can_frame* frame_;
};


}  // namespace tin


#endif  // TIN_FRAMEITEM_H
