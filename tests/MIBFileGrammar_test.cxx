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
      EXPORTS xd;
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
  EXPECT_EQ(result.exports.size(), 1);
  for (const auto& export_ : result.exports) {
    EXPECT_EQ(export_, "xd"s);
  }
  EXPECT_EQ(result.types.size(), 0);
  EXPECT_EQ(result.sequences.size(), 0);
}

TEST_F(MIBFileGrammar_test, minimal)
{
  string input {R"(
    Empty DEFINITIONS ::= BEGIN
      IMPORTS abc, def FROM ghi;
      EXPORTS abcex, defex;
      macro MACRO ::= BEGIN END
      qwe OBJECT IDENTIFIER ::= { rty 123 }
      ccc ::= SEQUENCE { ddd eee, fff ggg }
      vvv ::= INTEGER
      aaa OBJECT-TYPE ::= { bbb 333 }
      uio ::= SEQUENCE { asd fgh, zxc vbn }
      yyy ::= [PRIVATE 123] IMPLICIT INTEGER (0..22)
      choice ::= CHOICE { sel ect, ing stuff }
    END
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "Empty"s);
  EXPECT_EQ(result.imports.size(), 1);
  EXPECT_EQ(result.exports.size(), 2);
  EXPECT_EQ(result.types.size(), 2);
  EXPECT_EQ(result.types.at(1).name, "aaa"s);
  EXPECT_EQ(result.types.at(1).baseType.isObjectIdentifier, true);
  EXPECT_EQ(result.types.at(1).address.label, "bbb"s);
  EXPECT_EQ(result.types.at(1).address.value, 333);
  EXPECT_EQ(result.sequences.size(), 3);
  EXPECT_EQ(result.sequences.at(1).name, "uio"s);
  EXPECT_EQ(result.sequences.at(1).tag, "SEQUENCE"s);
  EXPECT_EQ(result.sequences.at(1).sequence.size(), 2);
  EXPECT_NE(result.sequences.at(1).sequence.find("zxc"), result.sequences.at(1).sequence.end());
  EXPECT_EQ(result.sequences.at(1).sequence["zxc"].name.name, "vbn"s);
  EXPECT_EQ(result.sequences.at(2).name, "choice"s);
  EXPECT_EQ(result.sequences.at(2).tag, "CHOICE"s);
  EXPECT_EQ(result.aliases.size(), 2);
  EXPECT_EQ(result.aliases.at(1).name, "yyy"s);
  EXPECT_EQ(result.aliases.at(1).isExplicit, false);
  EXPECT_EQ(result.aliases.at(1).isImplicit, true);
  EXPECT_EQ(result.aliases.at(1).visibility, "PRIVATE"s);
  EXPECT_EQ(result.aliases.at(1).typeIdentifier, 123);
  EXPECT_EQ(result.aliases.at(1).dataType.name.isInteger, true);
  EXPECT_EQ(result.aliases.at(1).dataType.restriction.range, true);
}
