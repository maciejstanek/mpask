#include "mpask/TypeDeclaration.hxx"

#include "mpask/Address.hxx"
#include "mpask/DataTypeName.hxx"
#include "mpask/DataType.hxx"

#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

using namespace std;
using namespace mpask;

class TypeDeclaration_test : public ::testing::Test
{
};

TEST_F(TypeDeclaration_test, ostream)
{
  Address a;
  a.label = "abc";
  a.intermediateNodes = {{"def", 123}, {"ghi", 456}};
  a.value = 789;

  DataTypeName n;
  n.name = "OCTET OF VIOLIN STRINGS";

  DataType t;
  // TODO

  TypeDeclaration d;
  d.name = "name";
  d.baseType = n;
  d.syntax = t;
  d.access = "access";
  d.status = "status";
  d.description = "not shown";
  d.indices = {"as", "df", "gh"};
  d.address = a;
  stringstream s;
  s << d;
	EXPECT_EQ(s.str(), "{"
    "\n  \"name\": \"name\","
    "\n  \"base_type\": \"OCTET OF VIOLIN STRINGS\","
    "\n  \"syntax\": {"
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
    "\n  },"
    "\n  \"access\": \"access\","
    "\n  \"status\": \"status\","
    "\n  \"indices\": [\"as\", \"df\", \"gh\"],"
    "\n  \"address\": ["
    "\n    \"abc\","
    "\n    [\"def\", 123],"
    "\n    [\"ghi\", 456],"
    "\n    789"
    "\n  ]"
    "\n}"
	);
}
