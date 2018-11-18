#include "mpask/AliasDeclarationGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace mpask;

class AliasDeclarationGrammar_test : public ::testing::Test
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

TEST_F(AliasDeclarationGrammar_test, basic)
{
  string input {R"(
    DisplayString ::=
      OCTET STRING
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "DisplayString"s);
  EXPECT_EQ(result.isExplicit, false);
  EXPECT_EQ(result.isImplicit, false);
  EXPECT_EQ(result.visibility, ""s);
  EXPECT_EQ(result.typeIdentifier, -1);
  EXPECT_EQ(result.dataType.name.isOctetString, true);
}

TEST_F(AliasDeclarationGrammar_test, long)
{
  string input {R"(
    abc ::=
      [UNIVERSAL 3]
        EXPLICIT INTEGER
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "abc"s);
  EXPECT_EQ(result.isExplicit, true);
  EXPECT_EQ(result.isImplicit, false);
  EXPECT_EQ(result.visibility, "UNIVERSAL"s);
  EXPECT_EQ(result.typeIdentifier, 3);
  EXPECT_EQ(result.dataType.name.isInteger, true);
}

TEST_F(AliasDeclarationGrammar_test, context_specific)
{
  string input {R"(
    Test ::=
      [CONTEXT-SPECIFIC 3]
        EXPLICIT INTEGER
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.visibility, "CONTEXT-SPECIFIC"s);
}

TEST_F(AliasDeclarationGrammar_test, private)
{
  string input {R"(
    Test ::=
      [PRIVATE 3]
        EXPLICIT INTEGER
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.visibility, "PRIVATE"s);
}

TEST_F(AliasDeclarationGrammar_test, long_2)
{
  string input {R"(
    IpAddress ::=
      [APPLICATION 0]
        IMPLICIT OCTET STRING (SIZE (4))
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "IpAddress"s);
  EXPECT_EQ(result.isExplicit, false);
  EXPECT_EQ(result.isImplicit, true);
  EXPECT_EQ(result.visibility, "APPLICATION"s);
  EXPECT_EQ(result.typeIdentifier, 0);
  EXPECT_EQ(result.dataType.name.isOctetString, true);
  EXPECT_EQ(result.dataType.restriction.size, true);
  EXPECT_EQ(result.dataType.restriction.range, false);
  EXPECT_EQ(result.dataType.restriction.left, 4);
}

TEST_F(AliasDeclarationGrammar_test, rfc1155)
{
  string input {R"(
    Counter ::= [APPLICATION 1] IMPLICIT INTEGER (0..4294967295)
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "Counter"s);
  EXPECT_EQ(result.isExplicit, false);
  EXPECT_EQ(result.isImplicit, true);
  EXPECT_EQ(result.visibility, "APPLICATION"s);
  EXPECT_EQ(result.typeIdentifier, 1);
  EXPECT_EQ(result.dataType.name.isInteger, true);
  EXPECT_EQ(result.dataType.restriction.size, false);
  EXPECT_EQ(result.dataType.restriction.range, true);
  EXPECT_EQ(result.dataType.restriction.left, 0);
  EXPECT_EQ(result.dataType.restriction.right, 4294967295);
}
