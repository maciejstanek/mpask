#include "mpask/ImportGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <tuple>
#include <iostream>

using namespace std;
using namespace mpask;

class ImportGrammar_test : public ::testing::Test
{
protected:
  pair<bool, vector<pair<vector<string>, string>>> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    ImportGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    vector<pair<vector<string>, string>> result;
    bool status = phrase_parse(iter, end, parser, space, result);
    return {status, result};
  }
};

TEST_F(ImportGrammar_test, basic)
{
  string input {R"(
    IMPORTS
      abc, DEF, ghi
        FROM some_source
      jkl
        FROM Another-Source;
  )"};
  auto [status, result] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(result[0].first.size(), 3);
  EXPECT_EQ(result[0].first[0], "abc"s);
  EXPECT_EQ(result[0].first[1], "DEF"s);
  EXPECT_EQ(result[0].first[2], "ghi"s);
  EXPECT_EQ(result[0].second, "some_source"s);
  EXPECT_EQ(result[1].first.size(), 1);
  EXPECT_EQ(result[1].first[0], "jkl"s);
  EXPECT_EQ(result[1].second, "Another-Source"s);
}
