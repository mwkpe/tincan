#ifndef TIN_TREEITEMID_H
#define TIN_TREEITEMID_H


namespace tin {


enum class Tree_item_id {
  Unspecified,
  Root,
  Empty,
  Can_frame,
  Can_signal,
  Can_frame_def,
  Can_signal_def,
  Value_def
};


}  // namespace tin


#endif  // TIN_TREEITEMID_H
