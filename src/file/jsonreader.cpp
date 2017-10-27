#include "jsonreader.h"


#include <fstream>
#include <experimental/filesystem>
#include "json.hpp"

#include "tincan/errors.h"
#include "tincan/canframedef.h"
#include "tincan/bussignaldef.h"


namespace fs = std::experimental::filesystem;
using json = nlohmann::json;


tin::Can_bus_def tin::read_json(std::string_view filepath)
{
  tin::Can_bus_def bus_def;

  std::ifstream fs{std::string{filepath}};
  if (!fs.is_open())
    throw tin::File_error{"Could not open file"};

  std::stringstream ss;
  ss << fs.rdbuf();
  fs.close();
  auto j  = json::parse(ss);
  if (j.empty())
    throw tin::File_error{"File is empty"};

  bus_def.source_name = fs::path{filepath}.filename().string();

  for (const auto& f : j["frames"]) {
    bus_def.frame_defs.emplace_back();
    auto& frame_def = bus_def.frame_defs.back();
    frame_def.id = f["id"];
    frame_def.dlc = f["dlc"];
    frame_def.name = f["name"];

    for (const auto& s : f["signals"]) {
      frame_def.bus_signal_defs.emplace_back();
      auto& signal_def = frame_def.bus_signal_defs.back();
      signal_def.order = static_cast<tin::Byte_order>(s["byte_order"]);
      signal_def.sign = static_cast<tin::Value_sign>(s["value_sign"]);
      signal_def.pos = s["pos"];
      signal_def.len = s["len"];
      signal_def.multiplex_switch = s["multiplex_switch"];
      signal_def.multiplex_value = s["multiplex_value"];
      signal_def.factor = s["factor"];
      signal_def.offset = s["offset"];
      signal_def.minimum = s["minimum"];
      signal_def.maximum = s["maximum"];
      //signal_def.unit = s["unit"];  // TODO: UTF-8 error
      signal_def.name = s["name"];
    }
  }

  return bus_def;
}
