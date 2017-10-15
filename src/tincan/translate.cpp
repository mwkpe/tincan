#include "translate.h"


#include "bussignaldef.h"
#include "canframedef.h"


tin::Can_bus_def tin::translate(const dbc::File& file)
{
  tin::Can_bus_def bus_def;
  bus_def.source_name = file.name;

  for (const dbc::Frame_def& fd : file.frame_defs) {
    bus_def.frame_defs.emplace_back();
    tin::Can_frame_def& frame_def = bus_def.frame_defs.back();
    frame_def.id = fd.id;
    frame_def.dlc = fd.dlc;
    frame_def.name = fd.name;

    for (const dbc::Signal_def& sd : fd.signal_defs) {
      frame_def.bus_signal_defs.emplace_back();
      tin::Bus_signal_def& signal_def = frame_def.bus_signal_defs.back();
      signal_def.multiplex_switch = sd.multiplex_switch;
      signal_def.order = sd.order == dbc::Byte_order::Intel ? tin::Byte_order::Intel :
          tin::Byte_order::Moto;
      signal_def.sign = sd.sign == dbc::Value_sign::Signed ? tin::Value_sign::Signed :
          tin::Value_sign::Unsigned;
      signal_def.multiplex_value = sd.multiplex_value;
      signal_def.pos = sd.pos;
      signal_def.len = sd.len;
      signal_def.factor = sd.factor;
      signal_def.offset = sd.offset;
      signal_def.minimum = sd.minimum;
      signal_def.maximum = sd.maximum;
      signal_def.unit = sd.unit;
      signal_def.name = sd.name;
    }
  }

  return bus_def;
}
