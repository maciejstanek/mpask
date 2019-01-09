#include "mpask/Dekober.hxx"

#include "mpask/Exception.hxx"

#include <gtest/gtest.h>
#include <iostream>

using namespace std;
using namespace mpask;

class Dekober_test : public ::testing::Test
{
};

TEST_F(Dekober_test, empty)
{
  auto result = Dekober({})();
  ASSERT_EQ(result, nullptr);
}

TEST_F(Dekober_test, integer)
{
  vector<unsigned char> code = {0x42, 0x01, 0x0f};
  auto result = Dekober(code)();
  ASSERT_NE(result, nullptr);
  cerr << "DECODED\n" <<  "-------\n" << *result << "\n";
}

TEST_F(Dekober_test, long_tag)
{
  vector<unsigned char> code = {0x5f, 0x87, 0xb9, 0xdd, 0xec, 0x40, 0x01, 0x0f};
  auto result = Dekober(code)();
  ASSERT_NE(result, nullptr);
  cerr << "DECODED\n" <<  "-------\n" << *result << "\n";
}

TEST_F(Dekober_test, long_length)
{
  vector<unsigned char> code = {0x30, 0x81, 0x81};
  for (int i = 0; i < 43; ++i) {
    code.push_back(0x42);
    code.push_back(0x01);
    code.push_back(0xff);
  }
  auto result = Dekober(code)();
  ASSERT_NE(result, nullptr);
  cerr << "DECODED\n" <<  "-------\n" << *result << "\n";
}

TEST_F(Dekober_test, octet_string)
{
  vector<unsigned char> code = {0x84, 0x04, 0x61, 0x62, 0x63, 0x64};
  auto result = Dekober(code)();
  ASSERT_NE(result, nullptr);
  cerr << "DECODED\n" <<  "-------\n" << *result << "\n";
}

TEST_F(Dekober_test, sequence)
{
  vector<unsigned char> code = {
    0x30, 0x06,
          0x42, 0x01, 0xdd,
          0x82, 0x01, 0xee};
  auto result = Dekober(code)();
  ASSERT_NE(result, nullptr);
  cerr << "DECODED\n" <<  "-------\n" << *result << "\n";
}

TEST_F(Dekober_test, sequence_in_sequence)
{
  vector<unsigned char> code = {
    0x30, 0x0f,
          0x23, 0x03,
                0x02, 0x01, 0xee,
          0x30, 0x08,
                0x41, 0x01, 0xcc,
                0xa2, 0x03, 0x02, 0x01, 0xdd};
  auto result = Dekober(code)();
  ASSERT_NE(result, nullptr);
  cerr << "DECODED\n" <<  "-------\n" << *result << "\n";
}

TEST_F(Dekober_test, example_1)
{
  vector<unsigned char> code = {
    0x30, 0x2c, 0x02, 0x01, 0x00,
    0x04, 0x07, 0x70, 0x72, 0x69,
    0x76, 0x61, 0x74, 0x65, 0xa0,
    0x1e, 0x02, 0x01, 0x01, 0x02,
    0x01, 0x00, 0x02, 0x01, 0x00,
    0x30, 0x13, 0x30, 0x11, 0x06,
    0x0d, 0x2b, 0x06, 0x01, 0x04,
    0x01, 0x94, 0x78, 0x01, 0x02,
    0x07, 0x03, 0x02, 0x00, 0x05,
    0x00};
  auto result = Dekober(code)();
  ASSERT_NE(result, nullptr);
  cerr << "DECODED\n" <<  "-------\n" << *result << "\n";
}
