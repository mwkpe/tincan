#ifndef TIN_TRANSLATE_H
#define TIN_TRANSLATE_H


// Translate data from source-specific formats to generic types


#include "canbusdef.h"
#include "file/dbcfile.h"


namespace tin
{


Can_bus_def translate(const dbc::File& file);


}  // namespace tin


#endif  // TIN_TRANSLATE_H
