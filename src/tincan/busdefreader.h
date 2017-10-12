#ifndef TIN_BUSDEFREADER_H
#define TIN_BUSDEFREADER_H


#include <string_view>
#include <stdexcept>

#include "tincan/canbusdef.h"


namespace tin
{



tin::Can_bus_def read_can_bus_def(std::string_view filepath);


}  // namespace tin


#endif  // TIN_BUSDEFREADER_H
