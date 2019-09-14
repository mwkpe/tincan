#ifndef TIN_JSONWRITER_H
#define TIN_JSONWRITER_H


#include <string_view>
#include "tincan/canbusdef.h"


namespace tin {


void write_json(const tin::Can_bus_def& bus_def, std::string_view filename);


}  // namespace tin


#endif  // TIN_JSONWRITER_H
