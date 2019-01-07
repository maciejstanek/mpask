#include "mpask/Address.hxx"

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
