#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN


#include "../ext/doctest.h"
#include "../src/file/dbcfile.h"
#include "../src/file/dbcparser.h"


using doctest::Approx;


TEST_CASE("DBC file parser")
{
  auto file = dbc::parse("test.dbc");
  CHECK(file.name == "test.dbc");

  const auto* frame_def = dbc::find_frame_def(file, "MESSAGE");
  REQUIRE(frame_def != nullptr);
  CHECK(frame_def->dlc == 4);
  CHECK(frame_def->id == 0xC9);
  CHECK(frame_def->signal_defs.size() == 4);

  SUBCASE("signal VEL")
  {
    const auto* signal_def = dbc::find_signal_def(*frame_def, "VEL");
    REQUIRE(signal_def != nullptr);
    CHECK(signal_def->pos == 12);
    CHECK(signal_def->len == 14);
    CHECK(signal_def->order == dbc::Byte_order::Intel);
    CHECK(signal_def->sign == dbc::Value_sign::Unsigned);
    CHECK(Approx(signal_def->factor) == 0.01220703125);
    CHECK(Approx(signal_def->offset) == 0);
    CHECK(Approx(signal_def->minimum) == 0);
    CHECK(Approx(signal_def->maximum) == 250);
    CHECK(signal_def->unit == "km/h");
  }

  SUBCASE("signal MODE")
  {
    const auto* signal_def = dbc::find_signal_def(*frame_def, "MODE");
    REQUIRE(signal_def != nullptr);
    CHECK(signal_def->pos == 26);
    CHECK(signal_def->len == 6);
    CHECK(signal_def->order == dbc::Byte_order::Intel);
    CHECK(signal_def->sign == dbc::Value_sign::Unsigned);
    CHECK(Approx(signal_def->factor) == 1);
    CHECK(Approx(signal_def->offset) == 0);
    CHECK(Approx(signal_def->minimum) == 10);
    CHECK(Approx(signal_def->maximum) == 50);
    CHECK(signal_def->unit == "");
  }
}
