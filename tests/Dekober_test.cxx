#include "mpask/Dekober.hxx"

#include "mpask/Exception.hxx"

#include <gtest/gtest.h>

using namespace std;
using namespace mpask;

class Dekober_test : public ::testing::Test
{
};

TEST_F(Dekober_test, empty)
{
  auto result = Dekober({})();
}

TEST_F(Dekober_test, integer)
{
  vector<unsigned char> code = {0x42, 0x01, 0x0f};
  auto result = Dekober(code)();
}

TEST_F(Dekober_test, long_tag)
{
  vector<unsigned char> code = {0x5f, 0x87, 0xb9, 0xdd, 0xec, 0x40, 0x01, 0x0f};
  auto result = Dekober(code)();
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
}
