#include "mpask/Restriction.hxx"

#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

using namespace std;
using namespace mpask;

class Restriction_test : public ::testing::Test
{
};

TEST_F(Restriction_test, ostream_null)
{
  Restriction r;
  stringstream s;
  s << r;
  EXPECT_EQ(s.str(), "{"
    "\n      \"size\": false,"
    "\n      \"range\": false,"
    "\n      \"left\": 0,"
    "\n      \"right\": 0"
    "\n    }"
  );
}

TEST_F(Restriction_test, ostream_not_null)
{
  Restriction r;
  r.size = true;
  r.range = true;
  r.left = 12;
  r.right = 21;
  stringstream s;
  s << r;
  EXPECT_EQ(s.str(), "{"
    "\n      \"size\": true,"
    "\n      \"range\": true,"
    "\n      \"left\": 12,"
    "\n      \"right\": 21"
    "\n    }"
  );
}
