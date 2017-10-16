#ifndef TIN_FRAMEITEM_H
#define TIN_FRAMEITEM_H


#include <cstdint>
#include <array>
#include <QVariant>
#include "treeitem.h"
#include "treeitemid.h"


namespace tin
{


struct Can_frame;


class Can_frame_item final : public Tree_item
{
public:
  Can_frame_item(const Can_frame* can_frame, Tree_item* parent = nullptr)
      : Tree_item{Item_id::Can_frame, parent}, can_frame_{can_frame}, cycle_times_{0} {}
  void update();
  const Can_frame* frame() const { return can_frame_; }
  virtual QVariant data(int column) const override;

private:
  const Can_frame* can_frame_;
  std::array<std::uint16_t, 16> cycle_times_;
  std::array<std::uint16_t, 16>::size_type cycle_times_pos_ = 0;
  int average_cycle_time_ = 0;
  std::uint64_t last_frame_time_ = 0;
};


}  // namespace tin


#endif  // TIN_FRAMEITEM_H
