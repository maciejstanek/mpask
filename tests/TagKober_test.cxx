#include "mpask/TagKober.hxx"

#include <gtest/gtest.h>

using namespace std;
using namespace mpask;

class TagKober_test : public ::testing::Test
{
};

TEST_F(TagKober_test, zero)
{
  auto coded = TagKober()(0);
  vector<unsigned char> golden {0x00};
  ASSERT_EQ(coded, golden);
}

TEST_F(TagKober_test, short_max)
{
  auto coded = TagKober()(30);
  vector<unsigned char> golden {0x1e};
  ASSERT_EQ(coded, golden);
}

TEST_F(TagKober_test, long_min)
{
  auto coded = TagKober()(31);
  vector<unsigned char> golden {0x1f, 0x1f};
  ASSERT_EQ(coded, golden);
}

TEST_F(TagKober_test, quite_long)
{
  auto coded = TagKober()(2'000'123'456);
  vector<unsigned char> golden {0x1f, 0x87, 0xb9, 0xdd, 0xec, 0x40};
  ASSERT_EQ(coded, golden);
}
