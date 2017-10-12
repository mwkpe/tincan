#ifndef TIN_BUSDEFWRITER_H
#define TIN_BUSDEFWRITER_H


#include <string_view>
#include "tincan/canbusdef.h"


namespace tin
{


void write_can_bus_def(const tin::Can_bus_def& bus_def, std::string_view filename);


}  // namespace tin


#endif  // TIN_BUSDEFWRITER_H