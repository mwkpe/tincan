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

sregex decimal_values = '(' >> (factor = -+_) >> ',' >> (offset = -+_) >> ')' >> blank
    >> '[' >> (minimum = -+_) >> '|' >> (maximum = -+_) >> ']';
sregex decimal = +_d >> !('.' >> (fractional = +_d)) >> !((set='e','E')
    >> (exponent = !(set='-','+') >> +_d));


}  // namespace


dbc::Signal_meta_data dbc::meta::parse_signal(std::string_view line)
{
  auto precision = [](auto&& match) -> std::int8_t {
    xpr::smatch number;
    if (xpr::regex_match(match.str(), number, decimal)) {
      auto exp = number[exponent].length() > 0 ? stoi(number[exponent].str()) : 0;
      auto prec = number[fractional].length() - exp;
      return prec < 0 ? 0 : prec;
    }
    return 0;
  };

  xpr::smatch values;
  if (xpr::regex_search(std::string{line}, values, decimal_values)) {
    Signal_meta_data meta_data;
    try {
      meta_data.factor_precision = precision(values[factor]);
      meta_data.offset_precision = precision(values[offset]);
      meta_data.minimum_precision = precision(values[minimum]);
      meta_data.maximum_precision = precision(values[maximum]);
      return meta_data;
    }
    catch (const std::logic_error& e) {
      return Signal_meta_data{};
    }
  }

  return Signal_meta_data{};
}
