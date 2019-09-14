#ifndef TIN_TRANSLATE_H
#define TIN_TRANSLATE_H


// Translate data from source-specific formats to generic types


#include "canbusdef.h"
#include "file/dbcfile.h"


namespace tin {


Can_bus_def to_can_bus_def(const dbc::File& file);
dbc::File to_dbc_file(const tin::Can_bus_def& bus_def);


}  // namespace tin


#endif  // TIN_TRANSLATE_H
