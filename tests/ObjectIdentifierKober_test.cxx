#include "mpask/ObjectIdentifierKober.hxx"

#include "mpask/Address.hxx"
#include "mpask/Exception.hxx"

#include <gtest/gtest.h>

using namespace std;
using namespace mpask;

class ObjectIdentifierKober_test : public ::testing::Test
{
};

TEST_F(ObjectIdentifierKober_test, empty_address)
{
  Address address;
  auto code = ObjectIdentifierKober()(address);
  decltype(code) golden = {0x06, 0x01, 0x00};
  EXPECT_EQ(code, golden);
}

TEST_F(ObjectIdentifierKober_test, one_small_value)
{
  Address address {"", {}, 2};
  auto code = ObjectIdentifierKober()(address);
  decltype(code) golden = {0x06, 0x01, 0x50};
  EXPECT_EQ(code, golden);
}

TEST_F(ObjectIdentifierKober_test, one_too_big_value)
{
  Address address {"", {}, 200};
  EXPECT_THROW(ObjectIdentifierKober()(address), Exception);
}

TEST_F(ObjectIdentifierKober_test, two_small_values)
{
  Address address {"", {{"", 2}}, 2};
  auto code = ObjectIdentifierKober()(address);
  decltype(code) golden = {0x06, 0x01, 0x52};
  EXPECT_EQ(code, golden);
}

TEST_F(ObjectIdentifierKober_test, two_too_big_values)
{
  Address address {"", {{"", 1}}, 200};
  EXPECT_THROW(ObjectIdentifierKober()(address), Exception);
}

TEST_F(ObjectIdentifierKober_test, multiple_values)
{
  Address address {"", {{"", 1}, {"", 2}, {"", 3}}, 4};
  auto code = ObjectIdentifierKober()(address);
  decltype(code) golden = {0x06, 0x03, 0x2a, 0x03, 0x04};
  EXPECT_EQ(code, golden);
}

TEST_F(ObjectIdentifierKober_test, multiple_values_one_big)
{
  Address address {"", {{"", 1}, {"", 2}, {"", 3333}}, 4};
  auto code = ObjectIdentifierKober()(address);
  decltype(code) golden = {0x06, 0x04, 0x2a, 0x9a, 0x05, 0x04};
  EXPECT_EQ(code, golden);
}

TEST_F(ObjectIdentifierKober_test, vector)
{
  vector<int> v {1, 2, 3};
  auto code = ObjectIdentifierKober()(v);
  decltype(code) golden = {0x06, 0x02, 0x2a, 0x03};
  EXPECT_EQ(code, golden);
}
