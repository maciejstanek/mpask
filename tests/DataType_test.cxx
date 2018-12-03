#include "mpask/DataType.hxx"

#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

using namespace std;
using namespace mpask;

class DataType_test : public ::testing::Test
{
};

TEST_F(DataType_test, ostream_null)
{
  DataType t;
  stringstream s;
  s << t;
  EXPECT_EQ(s.str(), "{"
    "\n    \"name\": \"\","
    "\n    \"variant\": \"\","
    "\n    \"integer_values\": {"
    "\n    },"
    "\n    \"restriction\": {"
    "\n      \"size\": false,"
    "\n      \"range\": false,"
    "\n      \"left\": 0,"
    "\n      \"right\": 0"
    "\n    }"
    "\n  }"
  );
}

TEST_F(DataType_test, ostream_not_null)
{
  DataType t;
  t.name.name = "example_data_type_name";
  t.variant = "seq";
  t.integerValues = {{"abc", 123}, {"def", 456}};
  // r.restriction = DEFAULT;
  stringstream s;
  s << t;
  EXPECT_EQ(s.str(), "{"
    "\n    \"name\": \"example_data_type_name\","
    "\n    \"variant\": \"seq\","
    "\n    \"integer_values\": {"
    "\n      \"abc\": 123,"
    "\n      \"def\": 456"
    "\n    },"
    "\n    \"restriction\": {"
    "\n      \"size\": false,"
    "\n      \"range\": false,"
    "\n      \"left\": 0,"
    "\n      \"right\": 0"
    "\n    }"
    "\n  }"
  );
}
