#include "dbcparser.h"


#include <fstream>
#include <boost/spirit/home/x3.hpp>


namespace x3 = boost::spirit::x3;
namespace latin1 = boost::spirit::x3::iso8859_1;


namespace parsers
{


using x3::int_;
using x3::ulong_;
using x3::float_;
using latin1::char_;
using latin1::space;

x3::rule<class signal, dbc::signal> const signal = "signal";
auto const signal_def =
  x3::lit("SG_")
  >> +char_("a-zA-Z0-9_") >> ':'
  >> int_ >> '|' >> int_ >> '@'
  >> int_ >> char_("+-")
  >> '(' >> float_ >> ',' >> float_ >> ')'
  >> '[' >> float_ >> '|' >> float_ >> ']'
  ;
BOOST_SPIRIT_DEFINE(signal);

x3::rule<class message, dbc::message_base> const message = "message";
auto const message_def = x3::lit("BO_") >> ulong_ >> +char_("a-zA-Z0-9_") >> ':' >> int_ ;
BOOST_SPIRIT_DEFINE(message);


}  // namespace parsers


namespace
{


inline bool is_signal(std::string_view line)
{
  return x3::phrase_parse(std::begin(line), std::end(line), x3::lit("SG_"), latin1::space);
}


inline bool is_message(std::string_view line)
{
  return x3::phrase_parse(std::begin(line), std::end(line), x3::lit("BO_"), latin1::space);
}


dbc::signal parse_signal(std::string_view line)
{
  dbc::signal signal;
  if (!x3::phrase_parse(std::begin(line), std::end(line), parsers::signal, latin1::space, signal))
    throw dbc::parse_error{"Format error"};
  return signal;
}


dbc::message parse_message(std::string_view line)
{
  dbc::message message;
  if (!x3::phrase_parse(std::begin(line), std::end(line), parsers::message, latin1::space,
          static_cast<dbc::message_base&>(message)))
    throw dbc::parse_error{"Format error"};
  return message;
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
    if (is_message(line))
      dbc_file.messages.push_back(parse_message(line));
    if (is_signal(line)) {
      if (dbc_file.messages.empty())
        throw parse_error{"Format error"};
      else
        dbc_file.messages.back().signals_.push_back(parse_signal(line));
    }
  }

  return dbc_file;
}
