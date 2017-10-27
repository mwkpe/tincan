#ifndef TIN_JSONREADER_H
#define TIN_JSONREADER_H


#include <string_view>
#include <stdexcept>

#include "tincan/canbusdef.h"


namespace tin
{


tin::Can_bus_def read_json(std::string_view filepath);


}  // namespace tin


#endif  // TIN_JSONREADER_H
