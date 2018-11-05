#include "mpask/DataTypeGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace mpask;

class DataTypeGrammar_test : public ::testing::Test
{
protected:
  pair<bool, DataType> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    DataTypeGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    DataType result;
    bool status = phrase_parse(iter, end, parser, space, result);
    return {status, result};
  }
};

TEST_F(DataTypeGrammar_test, basic)
{
  string input {"INTEGER"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name.name, "INTEGER"s);
  EXPECT_EQ(result.isSequence, false);
  EXPECT_EQ(result.restriction.left, 0);
  EXPECT_EQ(result.restriction.right, 0);
  EXPECT_EQ(result.restriction.size, false);
  EXPECT_EQ(result.restriction.range, false);
  EXPECT_EQ(result.integerValues.size(), 0);
}

TEST_F(DataTypeGrammar_test, basic_2)
{
  string input {"abc"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name.name, "abc"s);
  EXPECT_EQ(result.isSequence, false);
  EXPECT_EQ(result.restriction.left, 0);
  EXPECT_EQ(result.restriction.right, 0);
  EXPECT_EQ(result.restriction.size, false);
  EXPECT_EQ(result.restriction.range, false);
  EXPECT_EQ(result.integerValues.size(), 0);
}

TEST_F(DataTypeGrammar_test, sequence_of)
{
  string input {"SEQUENCE OF abc"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name.name, "abc"s);
  EXPECT_EQ(result.isSequence, true);
  EXPECT_EQ(result.restriction.left, 0);
  EXPECT_EQ(result.restriction.right, 0);
  EXPECT_EQ(result.restriction.size, false);
  EXPECT_EQ(result.restriction.range, false);
  EXPECT_EQ(result.restriction.left, 0);
  EXPECT_EQ(result.integerValues.size(), 0);
}

TEST_F(DataTypeGrammar_test, integer_with_values)
{
  string input {"INTEGER {a(1),b(2)}"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name.name, "INTEGER"s);
  EXPECT_EQ(result.isSequence, false);
  EXPECT_EQ(result.restriction.left, 0);
  EXPECT_EQ(result.restriction.right, 0);
  EXPECT_EQ(result.restriction.size, false);
  EXPECT_EQ(result.restriction.range, false);
  EXPECT_EQ(result.integerValues.size(), 2);
}

TEST_F(DataTypeGrammar_test, string)
{
  string input {"DisplayString (SIZE (1..10))"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name.name, "DisplayString"s);
  EXPECT_EQ(result.isSequence, false);
  EXPECT_EQ(result.restriction.left, 1);
  EXPECT_EQ(result.restriction.right, 10);
  EXPECT_EQ(result.restriction.size, true);
  EXPECT_EQ(result.restriction.range, true);
  EXPECT_EQ(result.integerValues.size(), 0);
}

TEST_F(DataTypeGrammar_test, object_identifier)
{
  string input {"OBJECT IDENTIFIER"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name.name, "OBJECT IDENTIFIER"s);
}
