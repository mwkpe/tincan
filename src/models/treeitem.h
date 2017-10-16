#ifndef TIN_TREEITEM_H
#define TIN_TREEITEM_H


#include <memory>
#include <vector>
#include <QVariant>
#include "treeitemid.h"


namespace tin
{


class Tree_item
{
public:
  Tree_item(Item_id id = Item_id::Unspecified, Tree_item* parent = nullptr)
      : id_{id}, parent_{parent} {}
  virtual ~Tree_item() {}

  Item_id id() const { return id_; }

  int row() const;
  int row(const Tree_item* item) const;

  int child_count() const { return static_cast<int>(children_.size()); }

  Tree_item* parent() const { return parent_; }
  Tree_item* child(int row) const;

  void add_child(std::unique_ptr<Tree_item>&& item) { children_.push_back(std::move(item)); }

  virtual QVariant data([[maybe_unused]] int column) const { return QVariant{}; }

protected:
  Item_id id_;
  Tree_item* parent_;
  std::vector<std::unique_ptr<Tree_item>> children_;
};


}  // namespace tin


#endif  // TIN_TREEITEM_H
