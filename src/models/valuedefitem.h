#ifndef TIN_VALUEDEFITEM_H
#define TIN_VALUEDEFITEM_H


#include <cstdint>
#include <string>

#include <QString>
#include <QVariant>

#include "models/treeitem.h"
#include "models/treeitemid.h"


namespace tin {


class Value_def_item final : public Tree_item
{
public:
  Value_def_item(std::int32_t value, const std::string& definition, Tree_item* parent = nullptr)
      : Tree_item{Tree_item_id::Value_def, parent}, value_{value},
        definition_{QString::fromStdString(definition)} {}
  virtual QVariant data(int column) const override;

private:
  std::int32_t value_;
  QString definition_;
};


}  // namespace tin


#endif  // TIN_VALUEDEFITEM_H
