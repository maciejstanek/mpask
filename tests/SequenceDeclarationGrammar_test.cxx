#include "mpask/SequenceDeclarationGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace mpask;

class SequenceDeclarationGrammar_test : public ::testing::Test
{
protected:
  pair<bool, SequenceDeclaration> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    SequenceDeclarationGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    SequenceDeclaration result;
    bool status = phrase_parse(iter, end, parser, space, result);
    return {status, result};
  }
};

TEST_F(SequenceDeclarationGrammar_test, basic)
{
  string input {R"(
    Abc ::=
      SEQUENCE {
        Def
          Defdef,
        Ghi
          Ghighi
      }
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "Abc"s);
  EXPECT_EQ(result.sequence.size(), 2);
  EXPECT_NE(result.sequence.find("Def"), result.sequence.end());
  EXPECT_EQ(result.sequence["Def"], "Defdef"s);
  EXPECT_NE(result.sequence.find("Ghi"), result.sequence.end());
  EXPECT_EQ(result.sequence["Ghi"], "Ghighi"s);
}

TEST_F(SequenceDeclarationGrammar_test, object_identifier)
{
  string input {R"(
    Abc ::=
      SEQUENCE {
        Def
          OBJECT IDENTIFIER,
        Ghi
          Ghighi
      }
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.sequence.size(), 2);
  EXPECT_NE(result.sequence.find("Def"), result.sequence.end());
  EXPECT_EQ(result.sequence["Def"], "OBJECT IDENTIFIER"s);
}
