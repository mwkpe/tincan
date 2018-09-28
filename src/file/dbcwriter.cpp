#include "dbcwriter.h"


#include <string>
#include <fstream>
#include <experimental/filesystem>

#include "fmt/fmtlib.h"


using namespace fmt::literals;


namespace
{


inline void write_header(std::ofstream& fs)
{
  // Empty version and required sections for normal DBC
  fs << "VERSION \"\"\n\nNS_:\n\nBS_:\n\n";
}


inline void write_nodes(std::ofstream& fs, const std::vector<std::string>& nodes)
{
  fs << "BU_:";
  if (!nodes.empty()) {
    fs << " ";
    auto last = std::end(nodes) - 1;
    for (auto it = std::begin(nodes); it != last; ++it)
      fs << *it << ',';
    fs << *last;
  }
  fs << "\n\n";
}


inline void write_frame_def(std::ofstream& fs, const dbc::Frame_def& fd)
{
  // BO_ id name: dlc transmitter
  fs << fmt::format(R"(BO_ {} {}: {} {})", fd.id, fd.name, fd.dlc,
      fd.transmitter.empty() ? "Vector__XXX" : fd.transmitter) << '\n';
}


inline void write_signal_def(std::ofstream& fs, const dbc::Signal_def& sd)
{
  auto to_string = [](auto&& receiver) {
    std::string s{receiver.front()};
    for (auto it = std::begin(receiver) + 1; it != std::end(receiver); ++it) {
      s += ',';
      s += *it;
    }
    return s;
  };

  // SG_ name : pos|len@order_and_type (factor,offset) [min|max] "unit" receiver{, receiver}
  fs << fmt::format(R"(  SG_ {} {}: {}|{}@{}{} ({:.{}f},{:.{}f}) [{:.{}f}|{:.{}f}] "{}" {})",
      sd.name,
      sd.multiplex_switch ? "M " : sd.multiplex_value >= 0 ? "m{} "_format(sd.multiplex_value) : "",
      sd.pos,
      sd.len,
      static_cast<int>(sd.order),
      sd.sign == dbc::Value_sign::Signed ? "-" : "+",
      sd.factor,
      sd.meta_data.factor_precision,
      sd.offset,
      sd.meta_data.offset_precision,
      sd.minimum,
      sd.meta_data.minimum_precision,
      sd.maximum,
      sd.meta_data.maximum_precision,
      sd.unit,
      sd.receiver.empty() ? "Vector__XXX" : to_string(sd.receiver)
      ) << '\n';
}


}  // namespace


void dbc::write(const File& dbc_file, std::string_view filepath)
{
  std::ofstream fs{std::string{filepath}};
  if (!fs.is_open())
    throw Write_error{"Could not open file"};

  write_header(fs);
  write_nodes(fs, dbc_file.nodes);

  for (const auto& frame_def : dbc_file.frame_defs) {
    write_frame_def(fs, frame_def);
    for (const auto& signal_def : frame_def.signal_defs) {
      write_signal_def(fs, signal_def);
    }
    fs  << '\n';
  }
}
