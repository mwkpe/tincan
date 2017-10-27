#include "dbcwriter.h"


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


inline void write_nodes(std::ofstream& fs)
{
  fs << "BU_: Vector__XXX\n\n";
}


inline void write_frame_def(std::ofstream& fs, const dbc::Frame_def& fd)
{
  // BO_ id name: dlc transmitter
  fs << fmt::format(R"(BO_ {} {}: {} {})", fd.id, fd.name, fd.dlc, "Vector__XXX") << '\n';
}


inline void write_signal_def(std::ofstream& fs, const dbc::Signal_def& sd)
{
  // SG_ name : pos|len@order_and_type (factor,offset) [min|max] "unit" receiver{, receiver}
  fs << fmt::format(R"(  SG_ {} {}: {}|{}@{}{} ({},{}) [{}|{}] "{}" {})",
      sd.name,
      sd.multiplex_switch ? "M " : sd.multiplex_value > 0 ? "m{} "_format(sd.multiplex_value) : "",
      sd.pos,
      sd.len,
      static_cast<int>(sd.order),
      sd.sign == dbc::Value_sign::Signed ? "-" : "+",
      sd.factor,
      sd.offset,
      sd.minimum,
      sd.maximum,
      sd.unit,
      "Vector__XXX"
      ) << '\n';
}


}  // namespace


void dbc::write(const File& dbc_file, std::string_view filepath)
{
  std::ofstream fs{std::string{filepath}};
  if (!fs.is_open())
    throw Write_error{"Could not open file"};

  write_header(fs);
  write_nodes(fs);

  for (const auto& frame_def : dbc_file.frame_defs) {
    write_frame_def(fs, frame_def);
    for (const auto& signal_def : frame_def.signal_defs) {
      write_signal_def(fs, signal_def);
    }
    fs  << '\n';
  }
}
