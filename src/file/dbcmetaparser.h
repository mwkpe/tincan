#ifndef DBC_METAPARSER_H
#define DBC_METAPARSER_H


// Parse additional data, e.g. required decimal precision of factor and offset


#include <string_view>
#include "dbcfile.h"


namespace dbc::meta
{


Signal_meta_data parse_signal(std::string_view line);


}  // namespace dbc::meta


#endif  // DBC_METAPARSER_H
