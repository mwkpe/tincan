#ifndef TIN_CANBUSMODEL_H
#define TIN_CANBUSMODEL_H


#include <cstdint>
#include <unordered_map>
#include <unordered_set>

#include "canframeitem.h"
#include "treemodel.h"


namespace tin
{


class Can_bus;


class Can_bus_model final : public Tree_model
{
  Q_OBJECT

public:
  explicit Can_bus_model(const Can_bus* can_bus, QObject* parent = nullptr);
  virtual void construct() override;
  void reset();

public slots:
  void update_data_deferred(std::uint32_t frame_id);
  void update_data(std::uint32_t frame_id);

private:
  Can_frame_item* find_item(std::uint32_t frame_id);

  const Can_bus* can_bus_;
  std::unordered_map<std::uint32_t, Can_frame_item*> frame_items_;
  std::unordered_set<std::uint32_t> updated_frames_;
};


}  // namespace tin


#endif  // TIN_CANBUSMODEL_H
