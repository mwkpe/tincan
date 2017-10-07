#include "signalitem.h"


#include <QString>
#include "dbcfile.h"


QVariant tin::Signal_item::data(int column) const
{
  if (!signal_def_)
    return QVariant{};

  switch (column) {
    case 0: return QString::fromStdString(signal_def_->name);
  }

  return QVariant{};
}
