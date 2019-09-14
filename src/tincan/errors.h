#ifndef TIN_ERRORS_H
#define TIN_ERRORS_H


#include <stdexcept>
#include <string>


namespace tin {


class File_error : public std::runtime_error
{
public:
  explicit File_error(const std::string& s) : std::runtime_error{s} {}
  explicit File_error(const char* s) : std::runtime_error{s} {}
};


}  // namespace tin


#endif  // TIN_ERRORS_H
