#include "mpask/RestrictionGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>

using namespace std;
using namespace mpask;

class RestrictionGrammar_test : public ::testing::Test
{
protected:
  pair<bool, Restriction> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    RestrictionGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    Restriction restriction;
    bool status = phrase_parse(iter, end, parser, space, restriction);
    return {status, restriction};
  }
};

TEST_F(RestrictionGrammar_test, size)
{
  string input {"(SIZE (4))"};
  auto [status, restriction] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(restriction.size, true);
  EXPECT_EQ(restriction.range, false);
  EXPECT_EQ(restriction.left, 4);
  EXPECT_EQ(restriction.right, 4);
}

TEST_F(RestrictionGrammar_test, size_range)
{
  string input {"(SIZE (12..34))"};
  auto [status, restriction] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(restriction.size, true);
  EXPECT_EQ(restriction.range, true);
  EXPECT_EQ(restriction.left, 12);
  EXPECT_EQ(restriction.right, 34);
}

TEST_F(RestrictionGrammar_test, range)
{
  string input {"(12..34)"};
  auto [status, restriction] = parse(input);
  EXPECT_EQ(status, true);
  EXPECT_EQ(restriction.size, false);
  EXPECT_EQ(restriction.range, true);
  EXPECT_EQ(restriction.left, 12);
  EXPECT_EQ(restriction.right, 34);
}

TEST_F(RestrictionGrammar_test, error)
{
  string input {"(SIZE (12..34 ERROR))"};
  auto [status, restriction] = parse(input);
  EXPECT_EQ(status, false);
  EXPECT_EQ(restriction.size, false);
  EXPECT_EQ(restriction.range, false);
  EXPECT_EQ(restriction.left, 0);
  EXPECT_EQ(restriction.right, 0);
}
