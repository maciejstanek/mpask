#include "mpask/MacroGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>

using namespace std;
using namespace mpask;

class MacroGrammar_test : public ::testing::Test
{
};

TEST_F(MacroGrammar_test, empty)
{
  string input {R"(
    Empty MACRO ::= BEGIN
    END
  )"};
  string::const_iterator iter = input.begin();
  string::const_iterator end = input.end();
  MacroGrammar<string::const_iterator> parser;
  boost::spirit::ascii::space_type space;
  string result;
  bool status = phrase_parse(iter, end, parser, space, result);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result, "Empty"s);
}

TEST_F(MacroGrammar_test, smi)
{
  string input {R"(
    OBJECT-TYPE MACRO ::=
    BEGIN
        TYPE NOTATION ::= "SYNTAX" type (TYPE ObjectSyntax)
                          "ACCESS" Access
                          "STATUS" Status
        VALUE NOTATION ::= value (VALUE ObjectName)
    
        Access ::= "read-only"
                        | "read-write"
                        | "write-only"
                        | "not-accessible"
        Status ::= "mandatory"
                        | "optional"
                        | "obsolete"
    END
  )"};
  string::const_iterator iter = input.begin();
  string::const_iterator end = input.end();
  MacroGrammar<string::const_iterator> parser;
  boost::spirit::ascii::space_type space;
  string result;
  bool status = phrase_parse(iter, end, parser, space, result);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result, "OBJECT-TYPE"s);
}
