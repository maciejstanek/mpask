#include "mpask/ObjectIdentifierGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace mpask;

class ObjectIdentifierGrammar_test : public ::testing::Test
{
protected:
  pair<bool, ObjectIdentifier> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    ObjectIdentifierGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    ObjectIdentifier result;
    bool status = phrase_parse(iter, end, parser, space, result);
    return {status, result};
  }
};

TEST_F(ObjectIdentifierGrammar_test, basic)
{
  string input {"def OBJECT IDENTIFIER ::= { abc ghi(456) 123 }"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.identifier, "def"s); 
  EXPECT_EQ(result.address.label, "abc"s);
  EXPECT_EQ(result.address.intermediateNodes.size(), 1);
  EXPECT_EQ(result.address.intermediateNodes.at(0).first, "ghi"s);
  EXPECT_EQ(result.address.intermediateNodes.at(0).second, 456);
  EXPECT_EQ(result.address.value, 123);
}
