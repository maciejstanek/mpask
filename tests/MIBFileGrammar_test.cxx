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
}
