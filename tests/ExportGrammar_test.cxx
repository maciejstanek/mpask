#include "mpask/ExportGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <tuple>
#include <iostream>

using namespace std;
using namespace mpask;

class ExportGrammar_test : public ::testing::Test
{
protected:
  pair<bool, vector<string>> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    ExportGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    vector<string> result;
    bool status = phrase_parse(iter, end, parser, space, result);
    return {status, result};
  }
};

TEST_F(ExportGrammar_test, basic)
{
  string input {R"(
    EXPORTS
      abc, DEF;
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(result.at(0), "abc"s);
  EXPECT_EQ(result.at(1), "DEF"s);
}
