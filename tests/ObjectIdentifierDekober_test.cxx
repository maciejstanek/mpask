#include "mpask/ObjectIdentifierDekober.hxx"

#include "mpask/Address.hxx"
#include "mpask/Exception.hxx"

#include <gtest/gtest.h>

using namespace std;
using namespace mpask;

class ObjectIdentifierDekober_test : public ::testing::Test
{
};

TEST_F(ObjectIdentifierDekober_test, empty_code)
{
  vector<unsigned char> code = {};
  EXPECT_THROW(ObjectIdentifierDekober()(code), Exception);
}

TEST_F(ObjectIdentifierDekober_test, one_small_value)
{
  vector<unsigned char> code = {0x50};
  auto address = ObjectIdentifierDekober()(code);
  Address golden {"", {{"", 2}}, 0}; // Shouldn't it be only first value
  EXPECT_EQ(address, golden);
}

TEST_F(ObjectIdentifierDekober_test, first_two_elements_too_big)
{
  vector<unsigned char> code = {0xff};
  EXPECT_THROW(ObjectIdentifierDekober()(code), Exception);
}

// TODO: Now SEGFAULT happens
// TEST_F(ObjectIdentifierDekober_test, not_ending_value)
// {
//   vector<unsigned char> code = {0x2a, 0x9a, 0x05, 0x84};
//   EXPECT_THROW(ObjectIdentifierDekober()(code), Exception);
// }

TEST_F(ObjectIdentifierDekober_test, two_small_values)
{
  vector<unsigned char> code = {0x52};
  auto address = ObjectIdentifierDekober()(code);
  Address golden {"", {{"", 2}}, 2};
  EXPECT_EQ(address, golden);
}

TEST_F(ObjectIdentifierDekober_test, multiple_values)
{
  vector<unsigned char> code = {0x2a, 0x03, 0x04};
  auto address = ObjectIdentifierDekober()(code);
  Address golden {"", {{"", 1}, {"", 2}, {"", 3}}, 4};
  EXPECT_EQ(address, golden);
}

TEST_F(ObjectIdentifierDekober_test, multiple_values_one_big)
{
  vector<unsigned char> code = {0x2a, 0x9a, 0x05, 0x04};
  auto address = ObjectIdentifierDekober()(code);
  Address golden {"", {{"", 1}, {"", 2}, {"", 3333}}, 4};
  EXPECT_EQ(address, golden);
}

TEST_F(ObjectIdentifierDekober_test, vector)
{
  vector<unsigned char> code = {0x06, 0x02, 0x2a, 0x03};
  auto v = ObjectIdentifierDekober().getVector(code);
  vector<int> golden {1, 2, 3};
  EXPECT_EQ(v, golden);
}
