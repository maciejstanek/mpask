#include "mpask/CommentSkipper.hxx"

#include "mpask/Exception.hxx"

#include <gtest/gtest.h>
#include <sstream>
#include <string>

using namespace std;
using namespace mpask;

class CommentSkipper_test : public ::testing::Test
{
};

TEST_F(CommentSkipper_test, no_comment)
{
  CommentSkipper skipper;
  stringstream o, i {R"(
    abc
  )"};
  stringstream r {R"(
    abc
  )"};
  skipper.skip(i, o);
  EXPECT_EQ(o.str(), r.str());
}

TEST_F(CommentSkipper_test, single_comment)
{
  CommentSkipper skipper;
  stringstream o, i {R"(
    abc -- TEST
  )"};
  stringstream r {R"(
    abc 
  )"};
  skipper.skip(i, o);
  EXPECT_EQ(o.str(), r.str());
}

TEST_F(CommentSkipper_test, complex_example)
{
  CommentSkipper skipper;
  stringstream o, i {R"(
    abc -- TEST
    def
    ghi-- TEST
    jkl --TEST
    mno -- TEST -- sdfs dsfsdfsd
  )"};
  stringstream r {R"(
    abc 
    def
    ghi
    jkl 
    mno 
  )"};
  skipper.skip(i, o);
  EXPECT_EQ(o.str(), r.str());
}
