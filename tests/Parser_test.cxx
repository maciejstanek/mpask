#include "mpask/Parser.hxx"

#include "mpask/Exception.hxx"

#include <gtest/gtest.h>
#include <sstream>

using namespace std;
using namespace mpask;

class Parser_test : public ::testing::Test
{
};

TEST_F(Parser_test, basic)
{
  // Parser p;
  // stringstream s {R"(
  //   abc OBJECT IDENTIFIER ::= { def 1 }
  // )"};
  // // stringstream s {R"(
  // //   system     OBJECT IDENTIFIER ::= { mib-2 1 }
  // // )"};
  // //interfaces OBJECT IDENTIFIER ::= { mib-2 2 }
  // //at         OBJECT IDENTIFIER ::= { mib-2 3 }
  // //ip         OBJECT IDENTIFIER ::= { mib-2 4 }
  // //icmp       OBJECT IDENTIFIER ::= { mib-2 5 }
  // //tcp        OBJECT IDENTIFIER ::= { mib-2 6 }
  // EXPECT_NO_THROW(p.parse(s));
}

TEST_F(Parser_test, simple_object_identifier)
{
  Parser p;
  stringstream s {R"(
    abc OBJECT IDENTIFIER ::= { def 1 }
  )"};
  EXPECT_NO_THROW(p.parse(s));
}

TEST_F(Parser_test, long_object_identifier)
{
  Parser p;
  stringstream s {R"(
    abc OBJECT IDENTIFIER ::= { def ghi(13) jkl(1) 213 }
  )"};
  EXPECT_NO_THROW(p.parse(s));
}

TEST_F(Parser_test, negative)
{
  Parser p;
  stringstream s {R"(tf sdohf kjdsf)"};
  EXPECT_THROW(p.parse(s), Exception);
}
