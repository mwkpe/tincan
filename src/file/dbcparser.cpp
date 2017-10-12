#include "dbcparser.h"


#include <cstdint>
#include <tuple>
#include <fstream>
#include <algorithm>
#include <experimental/filesystem>

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>


BOOST_FUSION_ADAPT_STRUCT(
  dbc::Signal_def,
  (std::string, name)
  (std::uint32_t, pos)
  (std::uint32_t, len)
  (dbc::Byte_order, order)
  (dbc::Value_sign, sign)
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


struct signs_ : x3::symbols<dbc::Value_sign>
{
  signs_()
  {
    add
      ("+", dbc::Value_sign::Unsigned)
      ("-", dbc::Value_sign::Signed)
    ;
  }
} signs;

struct oders_ : x3::symbols<dbc::Byte_order>
{
  oders_()
  {
    add
      ("0", dbc::Byte_order::Moto)
      ("1", dbc::Byte_order::Intel)
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


void sort_signals(dbc::Frame_def& frame_def)
{
  std::sort(std::begin(frame_def.signal_defs), std::end(frame_def.signal_defs),
      [](const auto& a, const auto& b){ return a.pos < b.pos; });
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
    if (auto [success, signal_def] = parse_signal_def(line); success) {
      if (dbc_file.frame_defs.empty())
        throw Parse_error{"Format error"};
      else
        dbc_file.frame_defs.back().signal_defs.push_back(signal_def);
    }
    else if (auto [success, frame_def] = parse_frame_def(line); success) {
      dbc_file.frame_defs.push_back(frame_def);
    }
    else if (messages_block_done(line)) {
      break;
    }
  }

  for (auto& frame_def : dbc_file.frame_defs)
    sort_signals(frame_def);

  return dbc_file;
}