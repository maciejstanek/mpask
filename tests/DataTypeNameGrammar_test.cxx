#include "mpask/DataTypeNameGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace mpask;

class DataTypeNameGrammar_test : public ::testing::Test
{
protected:
  pair<bool, DataTypeName> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    DataTypeNameGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    DataTypeName result;
    bool status = phrase_parse(iter, end, parser, space, result);
    return {status, result};
  }
};

TEST_F(DataTypeNameGrammar_test, object_identifier)
{
  string input {"OBJECT IDENTIFIER"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "OBJECT IDENTIFIER"s);
  EXPECT_EQ(result.isObjectIdentifier, true);
  EXPECT_EQ(result.isOctetString, false);
  EXPECT_EQ(result.isInteger, false);
  EXPECT_EQ(result.isNull, false);
}
TEST_F(DataTypeNameGrammar_test, octet_string)
{
  string input {"OCTET STRING"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "OCTET STRING"s);
  EXPECT_EQ(result.isObjectIdentifier, false);
  EXPECT_EQ(result.isOctetString, true);
  EXPECT_EQ(result.isInteger, false);
  EXPECT_EQ(result.isNull, false);
}

TEST_F(DataTypeNameGrammar_test, integer)
{
  string input {"INTEGER"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "INTEGER"s);
  EXPECT_EQ(result.isObjectIdentifier, false);
  EXPECT_EQ(result.isOctetString, false);
  EXPECT_EQ(result.isInteger, true);
  EXPECT_EQ(result.isNull, false);
}

TEST_F(DataTypeNameGrammar_test, null)
{
  string input {"NULL"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "NULL"s);
  EXPECT_EQ(result.isObjectIdentifier, false);
  EXPECT_EQ(result.isOctetString, false);
  EXPECT_EQ(result.isInteger, false);
  EXPECT_EQ(result.isNull, true);
}

TEST_F(DataTypeNameGrammar_test, custom)
{
  string input {"abc"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "abc"s);
  EXPECT_EQ(result.isObjectIdentifier, false);
  EXPECT_EQ(result.isOctetString, false);
  EXPECT_EQ(result.isInteger, false);
  EXPECT_EQ(result.isNull, false);
}
