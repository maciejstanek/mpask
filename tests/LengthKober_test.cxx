#include "mpask/LengthKober.hxx"

#include <gtest/gtest.h>

using namespace std;
using namespace mpask;

class LengthKober_test : public ::testing::Test
{
};

TEST_F(LengthKober_test, zero)
{
  auto coded = LengthKober()(0);
  vector<unsigned char> golden {0x00};
  ASSERT_EQ(coded, golden);
}

TEST_F(LengthKober_test, short_max)
{
  auto coded = LengthKober()(127);
  vector<unsigned char> golden {0x7f};
  ASSERT_EQ(coded, golden);
}

TEST_F(LengthKober_test, long_min)
{
  auto coded = LengthKober()(128);
  vector<unsigned char> golden {0x81, 0x80};
  ASSERT_EQ(coded, golden);
}

TEST_F(LengthKober_test, quite_long)
{
  auto coded = LengthKober()(2'000'000'000);
  vector<unsigned char> golden {0x84, 0x77, 0x35, 0x94, 0x00};
  ASSERT_EQ(coded, golden);
}
