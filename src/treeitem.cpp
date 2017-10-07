#include "treeitem.h"


#include <algorithm>


int tin::Tree_item::row() const
{
  if (parent_) {
    return parent_->row(this);
  }

  return -1;
}


int tin::Tree_item::row(const Tree_item* item) const
{
  auto it = std::find_if(std::begin(children_), std::end(children_),
      [item](const auto& c) { return c.get() == item; });
  if (it != std::end(children_))
    return std::distance(std::begin(children_), it);

  return -1;
}


tin::Tree_item* tin::Tree_item::child(int row) const
{
  return row >= 0 && row < static_cast<int>(children_.size()) ? children_[row].get() : nullptr;
}
