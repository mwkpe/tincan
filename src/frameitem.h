#ifndef TIN_FRAMEITEM_H
#define TIN_FRAMEITEM_H


#include <cstdint>
#include <QVariant>

#include "canframe.h"
#include "treeitem.h"

namespace dbc { struct Frame_def; }


namespace tin
{


class Frame_item final : public Tree_item
{
public:
  Frame_item(can::Frame frame, const dbc::Frame_def* frame_def = nullptr, Tree_item* parent = nullptr)
      : Tree_item{parent}, frame_{frame}, frame_def_{frame_def} {}
  void update_frame(can::Frame frame) { frame_ = frame; counter_++; }
  virtual QVariant data(int column) const override;

private:
  can::Frame frame_;
  int counter_ = 0;
  const dbc::Frame_def* frame_def_;
};


}  // namespace tin


#endif  // TIN_FRAMEITEM_H
