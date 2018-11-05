#include "mpask/MIBFileGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace mpask;

class MIBFileGrammar_test : public ::testing::Test
{
protected:
  pair<bool, MIBFile> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    MIBFileGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    MIBFile result;
    bool status = phrase_parse(iter, end, parser, space, result);
    return {status, result};
  }
};

TEST_F(MIBFileGrammar_test, empty)
{
  string input {R"(
    Empty DEFINITIONS ::= BEGIN
    END
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "Empty"s);
  EXPECT_EQ(result.imports.size(), 0);
  EXPECT_EQ(result.types.size(), 0);
  EXPECT_EQ(result.sequences.size(), 0);
}

TEST_F(MIBFileGrammar_test, imports)
{
  string input {R"(
    Empty DEFINITIONS ::= BEGIN
      IMPORTS abc, def FROM ghi;
    END
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "Empty"s);
  EXPECT_EQ(result.imports.size(), 1);
  for (const auto& import : result.imports) {
    EXPECT_EQ(import.second, "ghi"s);
    EXPECT_EQ(import.first.size(), 2);
    EXPECT_EQ(import.first.at(0), "abc"s);
    EXPECT_EQ(import.first.at(1), "def"s);
  }
  EXPECT_EQ(result.types.size(), 0);
  EXPECT_EQ(result.sequences.size(), 0);
}

TEST_F(MIBFileGrammar_test, minimal)
{
  string input {R"(
    Empty DEFINITIONS ::= BEGIN
      IMPORTS abc, def FROM ghi;
      qwe OBJECT IDENTIFIER ::= { rty 123 }
      ccc ::= SEQUENCE { ddd eee, fff ggg }
      aaa OBJECT-TYPE ::= { bbb 333 }
      uio ::= SEQUENCE { asd fgh, zxc vbn }
    END
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "Empty"s);
  EXPECT_EQ(result.imports.size(), 1);
  EXPECT_EQ(result.types.size(), 2);
  EXPECT_EQ(result.types.at(1).name, "aaa"s);
  EXPECT_EQ(result.types.at(1).baseType.isObjectIdentifier, true);
  EXPECT_EQ(result.types.at(1).address.label, "bbb"s);
  EXPECT_EQ(result.types.at(1).address.value, 333);
  EXPECT_EQ(result.sequences.size(), 2);
  EXPECT_EQ(result.sequences.at(1).name, "uio"s);
  EXPECT_EQ(result.sequences.at(1).sequence.size(), 2);
  EXPECT_NE(result.sequences.at(1).sequence.find("zxc"), result.sequences.at(1).sequence.end());
  EXPECT_EQ(result.sequences.at(1).sequence["zxc"].name, "vbn"s);
}
