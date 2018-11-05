#include "mpask/TypeDeclarationGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace mpask;

class TypeDeclarationGrammar_test : public ::testing::Test
{
protected:
  pair<bool, TypeDeclaration> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    TypeDeclarationGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    TypeDeclaration result;
    bool status = phrase_parse(iter, end, parser, space, result);
    return {status, result};
  }
};

TEST_F(TypeDeclarationGrammar_test, basic)
{
  string input {R"(
    Xyz OBJECT-TYPE
      SYNTAX INTEGER
      ACCESS not-accessible
      STATUS mandatory
      DESCRIPTION
        "Lorem ipsum
        dolor sit amet."
      INDEX { Qwerty }
      ::= { Abc Def(123) 456 }
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "Xyz"s);
  EXPECT_EQ(result.baseType, "OBJECT-TYPE"s);
  EXPECT_EQ(result.syntax.name.name, "INTEGER"s);
  EXPECT_EQ(result.access, "not-accessible"s);
  EXPECT_EQ(result.status, "mandatory"s);
  EXPECT_EQ(result.description, "Lorem ipsum\n        dolor sit amet."s);
  EXPECT_EQ(result.indices.size(), 1);
  EXPECT_EQ(result.indices.at(0), "Qwerty"s);
  EXPECT_EQ(result.address.label, "Abc"s);
  EXPECT_EQ(result.address.intermediateNodes.size(), 1);
  EXPECT_EQ(result.address.value, 456);
}

TEST_F(TypeDeclarationGrammar_test, reversed_order)
{
  string input {R"(
    a OBJECT-TYPE
      INDEX { b }
      DESCRIPTION "c"
      STATUS d
      ACCESS e
      SYNTAX f
      ::= { g 1 }
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "a"s);
  EXPECT_EQ(result.baseType, "OBJECT-TYPE"s);
  EXPECT_EQ(result.syntax.name.name, "f"s);
  EXPECT_EQ(result.access, "e"s);
  EXPECT_EQ(result.status, "d"s);
  EXPECT_EQ(result.description, "c"s);
  EXPECT_EQ(result.indices.size(), 1);
  EXPECT_EQ(result.indices.at(0), "b"s);
  EXPECT_EQ(result.address.label, "g"s);
  EXPECT_EQ(result.address.intermediateNodes.size(), 0);
  EXPECT_EQ(result.address.value, 1);
}

TEST_F(TypeDeclarationGrammar_test, object_identifier)
{
  string input {R"(
    Xyz OBJECT IDENTIFIER
      ::= { Abc Def(123) 456 }
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.name, "Xyz"s);
  EXPECT_EQ(result.baseType, "OBJECT IDENTIFIER"s);
  EXPECT_EQ(result.syntax.name.name, ""s);
  EXPECT_EQ(result.access, ""s);
  EXPECT_EQ(result.status, ""s);
  EXPECT_EQ(result.description, ""s);
  EXPECT_EQ(result.indices.size(), 0);
  EXPECT_EQ(result.address.label, "Abc"s);
  EXPECT_EQ(result.address.intermediateNodes.size(), 1);
  EXPECT_EQ(result.address.intermediateNodes.at(0).first, "Def"s);
  EXPECT_EQ(result.address.intermediateNodes.at(0).second, 123);
  EXPECT_EQ(result.address.value, 456);
}
