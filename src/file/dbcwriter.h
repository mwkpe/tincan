#ifndef DBC_WRITER_H
#define DBC_WRITER_H


#include <string>
#include <string_view>
#include <stdexcept>

#include "file/dbcfile.h"


namespace dbc {


class Write_error : public std::runtime_error
{
public:
  explicit Write_error(const std::string& s) : std::runtime_error{s} {}
  explicit Write_error(const char* s) : std::runtime_error{s} {}
};


void write(const File& dbc_file, std::string_view filepath);


}  // namespace dbc


#endif  // DBC_WRITER_H
