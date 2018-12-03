#include "mpask/Kober.hxx"

#include "mpask/AliasDeclarationGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace mpask;

class Kober_test : public ::testing::Test
{
protected:
  pair<bool, AliasDeclaration> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    AliasDeclarationGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    AliasDeclaration result;
    bool status = phrase_parse(iter, end, parser, space, result);
    return {status, result};
  }
};

TEST_F(Kober_test, octet_string)
{
  string input {"abc ::= OCTET STRING"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto coded = Kober(result)("abcd");
  vector<unsigned char> golden {};
  ASSERT_EQ(coded, golden);
}

TEST_F(Kober_test, integer)
{
  string input {"abc ::= INTEGER"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto coded = Kober(result)("1234");
  vector<unsigned char> golden {};
  ASSERT_EQ(coded, golden);
}

TEST_F(Kober_test, null)
{
  string input {"abc ::= NULL"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto coded = Kober(result)("");
  vector<unsigned char> golden {0x05, 0x00};
  ASSERT_EQ(coded, golden);
}
