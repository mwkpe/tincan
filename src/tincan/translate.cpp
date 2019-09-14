#include "translate.h"


#include "tincan/cansignaldef.h"
#include "tincan/canframedef.h"


tin::Can_bus_def tin::to_can_bus_def(const dbc::File& file)
{
  tin::Can_bus_def bus_def;
  bus_def.source_name = file.name;
  bus_def.nodes = file.nodes;

  for (const auto& fd : file.frame_defs) {
    bus_def.frame_defs.emplace_back();
    auto& frame_def = bus_def.frame_defs.back();
    frame_def.id = fd.id;
    frame_def.dlc = fd.dlc;
    frame_def.name = fd.name;
    frame_def.transmitter = fd.transmitter;
    frame_def.multiplexer = fd.multiplexer;

    for (const auto& sd : fd.signal_defs) {
      frame_def.can_signal_defs.emplace_back();
      auto& signal_def = frame_def.can_signal_defs.back();
      signal_def.order = sd.order == dbc::Byte_order::Intel ? tin::Byte_order::Intel :
          tin::Byte_order::Moto;
      signal_def.sign = sd.sign == dbc::Value_sign::Signed ? tin::Value_sign::Signed :
          tin::Value_sign::Unsigned;
      signal_def.multiplex_switch = sd.multiplex_switch;
      signal_def.multiplex_value = sd.multiplex_value;
      signal_def.pos = sd.pos;
      signal_def.len = sd.len;
      signal_def.factor = sd.factor;
      signal_def.offset = sd.offset;
      signal_def.minimum = sd.minimum;
      signal_def.maximum = sd.maximum;
      signal_def.unit = sd.unit;
      signal_def.name = sd.name;
      signal_def.receiver = sd.receiver;
      signal_def.value_definitions = sd.value_definitions;
      signal_def.meta_data.factor_precision = sd.meta_data.factor_precision;
      signal_def.meta_data.offset_precision = sd.meta_data.offset_precision;
      signal_def.meta_data.minimum_precision = sd.meta_data.minimum_precision;
      signal_def.meta_data.maximum_precision = sd.meta_data.maximum_precision;
    }
  }

  return bus_def;
}


dbc::File tin::to_dbc_file(const tin::Can_bus_def& bus_def)
{
  dbc::File dbc_file;
  dbc_file.nodes = bus_def.nodes;

  for (const auto& fd : bus_def.frame_defs) {
    dbc_file.frame_defs.emplace_back();
    auto& frame_def = dbc_file.frame_defs.back();
    frame_def.id = fd.id;
    frame_def.dlc = fd.dlc;
    frame_def.name = fd.name;
    frame_def.transmitter = fd.transmitter;
    frame_def.multiplexer = fd.multiplexer;

    for (const auto& sd : fd.can_signal_defs) {
      frame_def.signal_defs.emplace_back();
      auto& signal_def = frame_def.signal_defs.back();
      signal_def.order = sd.order == tin::Byte_order::Intel ? dbc::Byte_order::Intel :
          dbc::Byte_order::Moto;
      signal_def.sign = sd.sign == tin::Value_sign::Signed ? dbc::Value_sign::Signed :
          dbc::Value_sign::Unsigned;
      signal_def.multiplex_switch = sd.multiplex_switch;
      signal_def.multiplex_value = sd.multiplex_value;
      signal_def.pos = sd.pos;
      signal_def.len = sd.len;
      signal_def.factor = sd.factor;
      signal_def.offset = sd.offset;
      signal_def.minimum = sd.minimum;
      signal_def.maximum = sd.maximum;
      signal_def.unit = sd.unit;
      signal_def.name = sd.name;
      signal_def.receiver = sd.receiver;
      signal_def.value_definitions = sd.value_definitions;
      signal_def.meta_data.factor_precision = sd.meta_data.factor_precision;
      signal_def.meta_data.offset_precision = sd.meta_data.offset_precision;
      signal_def.meta_data.minimum_precision = sd.meta_data.minimum_precision;
      signal_def.meta_data.maximum_precision = sd.meta_data.maximum_precision;
    }
  }

  return dbc_file;
}
