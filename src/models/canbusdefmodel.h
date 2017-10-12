#ifndef TIN_CANBUSDEFMODEL_H
#define TIN_CANBUSDEFMODEL_H


#include <cstdint>
#include <unordered_map>


#include "treemodel.h"


namespace tin
{


class Can_bus_def;


class Can_bus_def_model final : public Tree_model
{
  Q_OBJECT

public:
  explicit Can_bus_def_model(QObject* parent = nullptr);
  virtual void construct() override;
  void set(const Can_bus_def* bus_def);

public slots:
  void reset();
};


}  // namespace tin


#endif  // TIN_CANBUSDEFMODEL_H
