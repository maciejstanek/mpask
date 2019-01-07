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
  decltype(code) golden = {0};
  EXPECT_EQ(code, golden);
}

TEST_F(ObjectIdentifierKober_test, one_small_value)
{
  Address address {"", {}, 2};
  auto code = ObjectIdentifierKober()(address);
  decltype(code) golden = {80};
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
  decltype(code) golden = {82};
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
  decltype(code) golden = {42, 3, 4};
  EXPECT_EQ(code, golden);
}

TEST_F(ObjectIdentifierKober_test, multiple_values_one_big)
{
  Address address {"", {{"", 1}, {"", 2}, {"", 3333}}, 4};
  auto code = ObjectIdentifierKober()(address);
  decltype(code) golden = {42, 26, 5, 4};
  EXPECT_EQ(code, golden);
}
