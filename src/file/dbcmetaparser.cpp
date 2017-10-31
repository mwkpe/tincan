#include "dbcmetaparser.h"


#include <cstdint>
#include <string>
#include <stdexcept>
#include <boost/xpressive/xpressive_static.hpp>


namespace xpr = boost::xpressive;


namespace
{


using namespace xpr;
mark_tag factor{1}, offset{2}, minimum{3}, maximum{4}, fractional{1}, exponent{2};

// Fields in DBC signal definition using double type
sregex decimal_values = '(' >> (factor = -+_) >> ',' >> (offset = -+_) >> ')' >> blank
    >> '[' >> (minimum = -+_) >> '|' >> (maximum = -+_) >> ']';

// Decimal number with optional fractional part and scientific notation
sregex decimal = +_d >> !('.' >> (fractional = +_d)) >> !((set='e','E')
    >> (exponent = !(set='-','+') >> +_d));


}  // namespace


dbc::Signal_meta_data dbc::meta::parse_signal(std::string_view line)
{
  // Calculate required decimal places for correct reproduction (not significant digits)
  auto fractional_precision = [](auto&& match) -> std::int8_t {
    xpr::smatch number;
    if (xpr::regex_match(match.str(), number, decimal)) {
      auto exp = number[exponent].length() > 0 ? std::stoi(number[exponent].str()) : 0;
      auto prec = number[fractional].length() - exp;
      return prec < 0 ? 0 : prec;  // Positive exponent may lead to negative precision, e.g 1.0e9
    }
    return 0;
  };

  xpr::smatch values;
  if (xpr::regex_search(std::string{line}, values, decimal_values)) {
    try {
      Signal_meta_data meta_data;
      meta_data.factor_precision = fractional_precision(values[factor]);
      meta_data.offset_precision = fractional_precision(values[offset]);
      meta_data.minimum_precision = fractional_precision(values[minimum]);
      meta_data.maximum_precision = fractional_precision(values[maximum]);
      return meta_data;
    }
    catch (const std::logic_error& e) {
      return Signal_meta_data{};
    }
  }

  return Signal_meta_data{};
}
