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
  string input {"abc ::= [UNIVERSAL 1] INTEGER"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto coded = Kober(result)("1234");
  vector<unsigned char> golden {0x02, 0x02, 0x04, 0xd2 };
  ASSERT_EQ(coded, golden);
}

TEST_F(Kober_test, integer_small)
{
  string input {"abc ::= [APPLICATION 1] INTEGER"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto coded = Kober(result)("7");
  vector<unsigned char> golden {0x42, 0x01, 0x07};
  ASSERT_EQ(coded, golden);
}

TEST_F(Kober_test, integer_big)
{
  string input {"abc ::= [CONTEXT-SPECIFIC 1] INTEGER"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto coded = Kober(result)("4294967295"); // 2^32-1
  vector<unsigned char> golden {0x82, 0x04, 0xff, 0xff, 0xff, 0xff};
  ASSERT_EQ(coded, golden);
}

TEST_F(Kober_test, integer_zero)
{
  string input {"abc ::= [PRIVATE 1] INTEGER"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto coded = Kober(result)("0");
  vector<unsigned char> golden {0xc2, 0x00};
  ASSERT_EQ(coded, golden);
}

TEST_F(Kober_test, null)
{
  string input {"abc ::= [APPLICATION 2] NULL"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto coded = Kober(result)("");
  vector<unsigned char> golden {0x45, 0x00};
  ASSERT_EQ(coded, golden);
}
