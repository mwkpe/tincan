#include "busdefwriter.h"


#include <fstream>
#include "json.hpp"

#include "tincan/errors.h"
#include "tincan/canframedef.h"
#include "tincan/bussignaldef.h"


using json = nlohmann::json;


void tin::write_can_bus_def(const tin::Can_bus_def& bus_def, std::string_view filepath)
{
  json j;
  j["version"] = "0.1";
  j["format"] = "CAN";
  auto frames = json::array();

  for (const auto& frame_def : bus_def.frame_defs) {
    json frame;
    frame["id"] = frame_def.id;
    frame["dlc"] = frame_def.dlc;
    frame["name"] = frame_def.name;
    auto bus_signals = json::array();

    for (const auto& signal_def : frame_def.bus_signal_defs) {
      json signal;
      signal["byte_order"] = static_cast<int>(signal_def.order);
      signal["value_sign"] = static_cast<int>(signal_def.sign);
      signal["pos"] = signal_def.pos;
      signal["len"] = signal_def.len;
      signal["multiplex_switch"] = signal_def.multiplex_switch;
      signal["multiplex_value"] = signal_def.multiplex_value;
      signal["factor"] = signal_def.factor;
      signal["offset"] = signal_def.offset;
      signal["minimum"] = signal_def.minimum;
      signal["maximum"] = signal_def.maximum;
      //signal["unit"] = signal_def.unit;  // TODO: UTF-8 error
      signal["name"] = signal_def.name;
      bus_signals.push_back(signal);
    }

    frame["signals"] = bus_signals;
    frames.push_back(frame);
  }

  j["frames"] = frames;

  std::ofstream fs{std::string{filepath}};
  if (!fs.is_open())
    throw tin::File_error{"Could not open file"};

  fs << j.dump(2);
}
