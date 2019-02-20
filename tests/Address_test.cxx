#include "mpask/Address.hxx"

#include "mpask/Exception.hxx"

#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

using namespace std;
using namespace mpask;

class Address_test : public ::testing::Test
{
};

TEST_F(Address_test, ostream_1)
{
  Address a;
  a.label = "abc";
  a.value = 123;
  stringstream s;
  s << a;
  EXPECT_EQ(s.str(), "["
    "\n    \"abc\","
    "\n    123"
    "\n  ]"
  );
}

TEST_F(Address_test, ostream_multi)
{
  Address a {"abc", {{"def", 123}, {"ghi", 456}}, 789};
  stringstream s;
  s << a;
  EXPECT_EQ(s.str(), "["
    "\n    \"abc\","
    "\n    [\"def\", 123],"
    "\n    [\"ghi\", 456],"
    "\n    789"
    "\n  ]"
  );
}

TEST_F(Address_test, equality)
{
  Address a1 {"aaa", {{"bbb", 111}, {"ccc", 222}}, 333};
  Address a2 {"aaa", {{"bbb", 111}, {"ccc", 222}}, 333};
  EXPECT_EQ(a1, a2);
}

TEST_F(Address_test, non_equal_1)
{
  Address a1 {"aaa", {{"bbb", 111}, {"ccc", 222}}, 333};
  Address a2 {"xxx", {{"bbb", 111}, {"ccc", 222}}, 333};
  EXPECT_NE(a1, a2);
}

TEST_F(Address_test, non_equal_2)
{
  Address a1 {"aaa", {{"bbb", 111}, {"ccc", 222}}, 333};
  Address a2 {"aaa", {{"bbb", 111}, {"ccc", 222}}, 999};
  EXPECT_NE(a1, a2);
}

TEST_F(Address_test, non_equal_3)
{
  Address a1 {"aaa", {{"bbb", 111}, {"ccc", 222}}, 333};
  Address a2 {"aaa", {{"bbb", 111}}, 333};
  EXPECT_NE(a1, a2);
}

TEST_F(Address_test, non_equal_4)
{
  Address a1 {"aaa", {{"bbb", 111}, {"ccc", 222}}, 333};
  Address a2 {"aaa", {{"bbb", 111}, {"xxx", 222}}, 333};
  EXPECT_NE(a1, a2);
}

TEST_F(Address_test, non_equal_5)
{
  Address a1 {"aaa", {{"bbb", 111}, {"ccc", 222}}, 333};
  Address a2 {"aaa", {{"bbb", 111}, {"ccc", 999}}, 333};
  EXPECT_NE(a1, a2);
}

TEST_F(Address_test, vect)
{
  Address a1 {{1, 2, 3, 4}};
  Address a2 {"iso", {{"", 2}, {"", 3}}, 4};
  EXPECT_EQ(a1, a2);
}

TEST_F(Address_test, vect_throw_iso)
{
  EXPECT_THROW(Address({2, 3, 4, 5}), Exception);
}

TEST_F(Address_test, vect_throw_len)
{
  EXPECT_THROW(Address({1}), Exception);
}
