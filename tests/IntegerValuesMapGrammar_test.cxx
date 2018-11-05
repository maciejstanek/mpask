#include "mpask/IntegerValuesMapGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace mpask;

class IntegerValuesMapGrammar_test : public ::testing::Test
{
protected:
  pair<bool, map<string, int>> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    IntegerValuesMapGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    map<string, int> mapping;
    bool status = phrase_parse(iter, end, parser, space, mapping);
    return {status, mapping};
  }
};

TEST_F(IntegerValuesMapGrammar_test, basic)
{
  string input {R"({
    a-B_c(1),
    d-E_f (2),
    g-H_i ( 3 )
  })"};
  auto [status, mapping] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_NE(mapping.find("a-B_c"), mapping.end());
  EXPECT_EQ(mapping["a-B_c"], 1);
  EXPECT_NE(mapping.find("d-E_f"), mapping.end());
  EXPECT_EQ(mapping["d-E_f"], 2);
  EXPECT_NE(mapping.find("g-H_i"), mapping.end());
  EXPECT_EQ(mapping["g-H_i"], 3);
  EXPECT_EQ(mapping.size(), 3);
}
