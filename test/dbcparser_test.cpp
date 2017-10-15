#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN


#include "../ext/doctest.h"
#include "../src/file/dbcfile.h"
#include "../src/file/dbcparser.h"


using doctest::Approx;


TEST_CASE("DBC file parser")
{
  auto file = dbc::parse("test.dbc");
  CHECK(file.name == "test.dbc");

  const auto* veh_state_def = dbc::find_frame_def(file, "VEH_STATE");
  REQUIRE(veh_state_def != nullptr);
  CHECK(veh_state_def->dlc == 8);
  CHECK(veh_state_def->id == 0xC9);
  CHECK(veh_state_def->signal_defs.size() == 4);

  SUBCASE("signal CRC")
  {
    const auto* crc_def = dbc::find_signal_def(*veh_state_def, "CRC");
    REQUIRE(crc_def != nullptr);
    CHECK(crc_def->pos == 0);
    CHECK(crc_def->len == 8);
    CHECK(crc_def->order == dbc::Byte_order::Intel);
    CHECK(crc_def->sign == dbc::Value_sign::Unsigned);
    CHECK(Approx(crc_def->factor) == 1);
    CHECK(Approx(crc_def->offset) == 0);
    CHECK(Approx(crc_def->minimum) == 0);
    CHECK(Approx(crc_def->maximum) == 255);
    CHECK(crc_def->unit == "");
  }

  SUBCASE("signal ALIV")
  {
    const auto* aliv_def = dbc::find_signal_def(*veh_state_def, "ALIV");
    REQUIRE(aliv_def != nullptr);
    CHECK(aliv_def->pos == 8);
    CHECK(aliv_def->len == 4);
    CHECK(aliv_def->order == dbc::Byte_order::Intel);
    CHECK(aliv_def->sign == dbc::Value_sign::Unsigned);
    CHECK(Approx(aliv_def->factor) == 1);
    CHECK(Approx(aliv_def->offset) == 0);
    CHECK(Approx(aliv_def->minimum) == 0);
    CHECK(Approx(aliv_def->maximum) == 15);
    CHECK(aliv_def->unit == "");
  }

  SUBCASE("signal VELOCITY")
  {
    const auto* velocity_def = dbc::find_signal_def(*veh_state_def, "VELOCITY");
    REQUIRE(velocity_def != nullptr);
    CHECK(velocity_def->pos == 12);
    CHECK(velocity_def->len == 14);
    CHECK(velocity_def->order == dbc::Byte_order::Intel);
    CHECK(velocity_def->sign == dbc::Value_sign::Unsigned);
    CHECK(Approx(velocity_def->factor) == 0.01);
    CHECK(Approx(velocity_def->offset) == 0);
    CHECK(Approx(velocity_def->minimum) == 0);
    CHECK(Approx(velocity_def->maximum) == 140);
    CHECK(velocity_def->unit == "mph");
  }

  SUBCASE("signal WIPER_POSITION")
  {
    const auto* wiper_position_def = dbc::find_signal_def(*veh_state_def, "WIPER_POSITION");
    REQUIRE(wiper_position_def != nullptr);
    CHECK(wiper_position_def->pos == 26);
    CHECK(wiper_position_def->len == 16);
    CHECK(wiper_position_def->order == dbc::Byte_order::Intel);
    CHECK(wiper_position_def->sign == dbc::Value_sign::Unsigned);
    CHECK(Approx(wiper_position_def->factor) == 0.010986328125);
    CHECK(Approx(wiper_position_def->offset) == 0);
    CHECK(Approx(wiper_position_def->minimum) == 0);
    CHECK(Approx(wiper_position_def->maximum) == 180);
    CHECK(wiper_position_def->unit == "deg");
  }
}
