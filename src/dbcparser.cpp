#include "dbcparser.h"


#include <cstdint>
#include <tuple>
#include <fstream>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>


BOOST_FUSION_ADAPT_STRUCT(
  dbc::signal,
  (std::string, name)
  (std::uint32_t, pos)
  (std::uint32_t, len)
  (dbc::byte_order, order)
  (dbc::value_sign, sign)
  (double, factor)
  (double, offset)
  (double, minimum)
  (double, maximum)
  (std::string, unit)
)

BOOST_FUSION_ADAPT_STRUCT(
  dbc::message_base,
  (std::uint32_t, id)
  (std::string, name)
  (std::uint32_t, dlc)
)


namespace x3 = boost::spirit::x3;
namespace latin1 = boost::spirit::x3::iso8859_1;


struct signs_ : x3::symbols<dbc::value_sign>
{
  signs_()
  {
    add
      ("+", dbc::value_sign::unsigned_)
      ("-", dbc::value_sign::signed_)
    ;
  }
} signs;

struct oders_ : x3::symbols<dbc::byte_order>
{
  oders_()
  {
    add
      ("0", dbc::byte_order::moto)
      ("1", dbc::byte_order::intel)
    ;
  }
} orders;


namespace parsers
{


using x3::ulong_;
using x3::double_;
using latin1::char_;
using latin1::space;


const auto quoted_string = x3::lexeme['"' >> *(char_ - '"') >> '"'];


x3::rule<class signal, dbc::signal> const signal = "signal";
auto const signal_def =
  x3::lit("SG_")
  >> +char_("a-zA-Z0-9_") >> ':'
  >> ulong_ >> '|' >> ulong_ >> '@'
  >> orders >> signs
  >> '(' >> double_ >> ',' >> double_ >> ')'
  >> '[' >> double_ >> '|' >> double_ >> ']'
  >> quoted_string
  ;
BOOST_SPIRIT_DEFINE(signal);

x3::rule<class message, dbc::message_base> const message = "message";
auto const message_def = x3::lit("BO_") >> ulong_ >> +char_("a-zA-Z0-9_") >> ':' >> ulong_;
BOOST_SPIRIT_DEFINE(message);


}  // namespace parsers


namespace
{


inline bool messages_block_done(std::string_view line)
{
  // We're not interested in comments, etc.
  using latin1::space;
  using x3::lit;
  return x3::parse(std::begin(line), std::end(line), *space
      >> (lit("CM_") | lit("BA_DEF_") | lit("BA_")) >> space);
}


std::tuple<bool, dbc::signal> parse_signal(std::string_view line)
{
  dbc::signal signal;
  bool success = x3::phrase_parse(std::begin(line), std::end(line), parsers::signal, latin1::space,
      signal);
  return {success, signal};
}


std::tuple<bool, dbc::message> parse_message(std::string_view line)
{ 
  dbc::message message;
  bool success = x3::phrase_parse(std::begin(line), std::end(line), parsers::message, latin1::space,
      static_cast<dbc::message_base&>(message));
  return {success, message};
}


}  // namespace


dbc::file dbc::parse(std::string_view filename)
{
  file dbc_file;

  std::ifstream fs{std::string{filename}};
  if (!fs.is_open())
    throw parse_error{"Could not open file"};

  std::string line;
  while (std::getline(fs, line)) {
    if (auto [success, signal] = parse_signal(line); success) {
      if (dbc_file.messages.empty())
        throw parse_error{"Format error"};
      else
        dbc_file.messages.back().signals_.push_back(signal);
    }
    else if (auto [success, message] = parse_message(line); success) {
      dbc_file.messages.push_back(message);
    }
    else if (messages_block_done(line)) {
      return dbc_file;
    }
  }

  return dbc_file;
}
