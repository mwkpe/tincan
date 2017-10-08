#include "dbcparser.h"


#include <cstdint>
#include <tuple>
#include <fstream>
#include <experimental/filesystem>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>


BOOST_FUSION_ADAPT_STRUCT(
  dbc::Signal_def,
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
  dbc::Frame_def,
  (std::uint32_t, id)
  (std::string, name)
  (std::uint32_t, dlc)
  (std::vector<dbc::Signal_def>, signal_defs)
)


namespace fs = std::experimental::filesystem;
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


x3::rule<class signal, dbc::Signal_def> const signal = "signal";
const auto signal_def =
    x3::lit("SG_")
    >> +char_("a-zA-Z0-9_") >> ':'
    >> ulong_ >> '|' >> ulong_ >> '@'
    >> orders >> signs
    >> '(' >> double_ >> ',' >> double_ >> ')'
    >> '[' >> double_ >> '|' >> double_ >> ']'
    >> quoted_string
    ;
BOOST_SPIRIT_DEFINE(signal);

x3::rule<class message, dbc::Frame_def> const frame = "frame";
const auto frame_def = x3::lit("BO_") >> ulong_ >> +char_("a-zA-Z0-9_") >> ':' >> ulong_
    >> x3::attr(std::vector<dbc::Signal_def>());
BOOST_SPIRIT_DEFINE(frame);


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


std::tuple<bool, dbc::Signal_def> parse_signal_def(std::string_view line)
{
  dbc::Signal_def signal;
  bool success = x3::phrase_parse(std::begin(line), std::end(line), parsers::signal, latin1::space,
      signal);
  return {success, signal};
}


std::tuple<bool, dbc::Frame_def> parse_frame_def(std::string_view line)
{ 
  dbc::Frame_def message;
  bool success = x3::phrase_parse(std::begin(line), std::end(line), parsers::frame, latin1::space,
      message);
  return {success, message};
}


}  // namespace


dbc::File dbc::parse(std::string_view filepath)
{
  File dbc_file;

  std::ifstream fs{std::string{filepath}};
  if (!fs.is_open())
    throw Parse_error{"Could not open file"};

  dbc_file.name = fs::path{filepath}.filename().string();

  std::string line;
  while (std::getline(fs, line)) {
    if (auto [success, Signal_def] = parse_signal_def(line); success) {
      if (dbc_file.frame_defs.empty())
        throw Parse_error{"Format error"};
      else
        dbc_file.frame_defs.back().signal_defs.push_back(Signal_def);
    }
    else if (auto [success, Frame_def] = parse_frame_def(line); success) {
      dbc_file.frame_defs.push_back(Frame_def);
    }
    else if (messages_block_done(line)) {
      return dbc_file;
    }
  }

  return dbc_file;
}
