#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN


#include "../ext/doctest.h"
#include "../src/dbcfile.h"
#include "../src/dbcparser.h"


using doctest::Approx;


TEST_CASE("DBC file parser")
{
  auto file = dbc::parse("test.dbc");

  auto message_it = dbc::find_message(file, "MESSAGE");
  REQUIRE(message_it != std::end(file.messages));
  auto& m = *message_it;
  CHECK(m.dlc == 4);
  CHECK(m.id == 0xC9);
  CHECK(m.signals_.size() == 4);

  SUBCASE("signal VEL")
  {
    auto signal_it = dbc::find_signal(m, "VEL");
    REQUIRE(signal_it != std::end(m.signals_));
    auto signal = *signal_it;
    CHECK(signal.pos == 12);
    CHECK(signal.len == 14);
    CHECK(signal.order == dbc::byte_order::intel);
    CHECK(signal.sign == dbc::value_sign::unsigned_);
    CHECK(Approx(signal.factor) == 0.01220703125);
    CHECK(Approx(signal.offset) == 0);
    CHECK(Approx(signal.minimum) == 0);
    CHECK(Approx(signal.maximum) == 250);
    CHECK(signal.unit == "km/h");
  }

  SUBCASE("signal MODE")
  {
    auto signal_it = dbc::find_signal(m, "MODE");
    REQUIRE(signal_it != std::end(m.signals_));
    auto signal = *signal_it;
    CHECK(signal.pos == 26);
    CHECK(signal.len == 6);
    CHECK(signal.order == dbc::byte_order::intel);
    CHECK(signal.sign == dbc::value_sign::unsigned_);
    CHECK(Approx(signal.factor) == 1);
    CHECK(Approx(signal.offset) == 0);
    CHECK(Approx(signal.minimum) == 10);
    CHECK(Approx(signal.maximum) == 50);
    CHECK(signal.unit == "");
  }
}
