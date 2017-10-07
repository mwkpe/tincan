#ifndef TIN_FRAMEMODEL_H
#define TIN_FRAMEMODEL_H


#include <cstdint>
#include <unordered_map>

#include "frameitem.h"
#include "treemodel.h"

namespace can { class Bus; }
namespace dbc { class File; }


namespace tin
{


class Frame_model : public Tree_model
{
  Q_OBJECT

public:
  explicit Frame_model(const can::Bus* can_bus, const dbc::File* dbc_file = nullptr,
      QObject* parent = nullptr);
  virtual void construct() override;
  void reset();

public slots:
  void update_frame(std::uint32_t id);

private:
  Frame_item* find_item(std::uint32_t frame_id);

  const can::Bus* can_bus_;
  const dbc::File* dbc_file_;
  std::unordered_map<std::uint32_t, Frame_item*> frame_items_;
};


}  // namespace tin


#endif  // TIN_FRAMEMODEL_H
