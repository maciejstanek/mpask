#include "mpask/AddressGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace mpask;

class AddressGrammar_test : public ::testing::Test
{
protected:
  pair<bool, Address> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    AddressGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    Address result;
    bool status = phrase_parse(iter, end, parser, space, result);
    return {status, result};
  }
};

TEST_F(AddressGrammar_test, basic)
{
  string input {"{ abc 123 }"};
  auto [status, address] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(address.label, "abc"s); 
  EXPECT_EQ(address.intermediateNodes.size(), 0);
  EXPECT_EQ(address.value, 123);
}

TEST_F(AddressGrammar_test, intermediate_nodes)
{
  string input {"{ abc def(456) ghi ( 789 ) 123 }"};
  auto [status, address] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(address.label, "abc"s); 
  EXPECT_EQ(address.intermediateNodes.size(), 2);
  EXPECT_EQ(address.intermediateNodes[0].first, "def"s);
  EXPECT_EQ(address.intermediateNodes[0].second, 456);
  EXPECT_EQ(address.intermediateNodes[1].first, "ghi"s);
  EXPECT_EQ(address.intermediateNodes[1].second, 789);
  EXPECT_EQ(address.value, 123);
}
