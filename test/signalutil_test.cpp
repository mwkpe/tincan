#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN


#include <cstdint>
#include <variant>
#include <array>

#include "../ext/doctest.h"
#include "../src/tincan/signalutil.h"


using doctest::Approx;


TEST_CASE("pswap64")
{
  // Bit positions of first byte after swapping an 8-byte block
  CHECK(tin::pswap64(0) == 56);
  CHECK(tin::pswap64(1) == 57);
  CHECK(tin::pswap64(2) == 58);
  CHECK(tin::pswap64(3) == 59);
  CHECK(tin::pswap64(4) == 60);
  CHECK(tin::pswap64(5) == 61);
  CHECK(tin::pswap64(6) == 62);
  CHECK(tin::pswap64(7) == 63);

  // Bit positions of second byte after swapping an 8-byte block
  CHECK(tin::pswap64(8) == 48);
  CHECK(tin::pswap64(9) == 49);
  CHECK(tin::pswap64(10) == 50);
  CHECK(tin::pswap64(11) == 51);
  CHECK(tin::pswap64(12) == 52);
  CHECK(tin::pswap64(13) == 53);
  CHECK(tin::pswap64(14) == 54);
  CHECK(tin::pswap64(15) == 55);

  // Bit positions of last byte after swapping an 8-byte block
  CHECK(tin::pswap64(56) == 0);
  CHECK(tin::pswap64(57) == 1);
  CHECK(tin::pswap64(58) == 2);
  CHECK(tin::pswap64(59) == 3);
  CHECK(tin::pswap64(60) == 4);
  CHECK(tin::pswap64(61) == 5);
  CHECK(tin::pswap64(62) == 6);
  CHECK(tin::pswap64(63) == 7);
}


TEST_CASE("raw_value")
{
  std::array<std::uint8_t, 8> buffer{0};
  buffer[1] = 0b1100'0000;
  buffer[2] = 0b1000'1110;
  buffer[3] = 0b0001'1011;

  SUBCASE("shifting")
  {
    auto order = tin::Byte_order::Intel;
    auto sign = tin::Value_sign::Unsigned;

    CHECK(std::get<std::uint64_t>(raw_value(buffer, 14, 15, order, sign)) == 28219);
    // Zero in the least significant bit
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 13, 16, order, sign)) == 56438);
    // Zero in more significant bits
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 14, 16, order, sign)) == 28219);
    // Single-byte
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 0, 8, order, sign)) == 0);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 8, 6, order, sign)) == 0);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 8, 7, order, sign)) == 64);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 8, 8, order, sign)) == 192);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 10, 6, order, sign)) == 48);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 18, 6, order, sign)) == 35);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 24, 8, order, sign)) == 27);
    // Multi-byte
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 0, 16, order, sign)) == 49152);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 0, 24, order, sign)) == 9355264);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 0, 32, order, sign)) == 462340096);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 16, 24, order, sign)) == 7054);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 19, 5, order, sign)) == 17);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 17, 8, order, sign)) == 199);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 22, 9, order, sign)) == 110);
  }

  SUBCASE("signed values")
  {
    auto order = tin::Byte_order::Intel;
    auto sign = tin::Value_sign::Signed;

    CHECK(std::get<std::int64_t>(raw_value(buffer, 0, 8, order, sign)) == -256);
    CHECK(std::get<std::int64_t>(raw_value(buffer, 8, 8, order, sign)) == -64);
    CHECK(std::get<std::int64_t>(raw_value(buffer, 16, 8, order, sign)) == -114);
    CHECK(std::get<std::int64_t>(raw_value(buffer, 17, 3, order, sign)) == -1);
    CHECK(std::get<std::int64_t>(raw_value(buffer, 16, 4, order, sign)) == -2);
    CHECK(std::get<std::int64_t>(raw_value(buffer, 0, 27, order, sign)) == -74530816);
  }

  SUBCASE("moto byte order")
  {
    auto order = tin::Byte_order::Moto;
    auto sign = tin::Value_sign::Unsigned;

    CHECK(std::get<std::uint64_t>(raw_value(buffer, 7, 8, order, sign)) == 0);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 15, 8, order, sign)) == 192);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 11, 9, order, sign)) == 17);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 14, 10, order, sign)) == 516);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 15, 22, order, sign)) == 3154822);
    CHECK(std::get<std::uint64_t>(raw_value(buffer, 17, 3, order, sign)) == 4);
  }
}
