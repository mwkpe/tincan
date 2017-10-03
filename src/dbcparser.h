#ifndef DBC_PARSER_H
#define DBC_PARSER_H


#include <string>
#include <string_view>
#include <stdexcept>

#include "dbcfile.h"


namespace dbc
{


class parse_error : public std::runtime_error
{
public:
  parse_error(const std::string& s) : std::runtime_error{s} {}
  parse_error(const char* s) : std::runtime_error{s} {}
};


file parse(std::string_view filepath);


}  // namespace dbc


#endif  // DBC_PARSER_H
