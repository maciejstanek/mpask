#include "mpask/Parser.hxx"

#include "mpask/Exception.hxx"

#include <gtest/gtest.h>
#include <sstream>
#include <fstream>

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
    system OBJECT IDENTIFIER ::= { mib-2 1 }
  )"};
  EXPECT_NO_THROW(p.parse(s));
}

TEST_F(Parser_test, long_object_identifier)
{
  Parser p;
  stringstream s {R"(
    internet OBJECT IDENTIFIER ::= { iso org(3) dod(6) 1 }
  )"};
  // TODO: The parser should not allow for spaces before and inside the parenthesis.
  EXPECT_NO_THROW(p.parse(s));
}

TEST_F(Parser_test, simple_object_type)
{
  Parser p;
  stringstream s {R"(
    sysUpTime OBJECT-TYPE
      ::= { system 3 }
  )"};
  EXPECT_NO_THROW(p.parse(s));
}

TEST_F(Parser_test, long_object_type)
{
  Parser p;
  stringstream s {R"(
    sysUpTime OBJECT-TYPE
      SYNTAX TimeTicks
      ACCESS read-only
      STATUS mandatory
      DESCRIPTION
        "The time (in hundredths of a second) since the
        network management portion of the system was last
        re-initialized."
      ::= { system 3 }
  )"};
  EXPECT_NO_THROW(p.parse(s));
}

TEST_F(Parser_test, simple_data_type)
{
  Parser p;
  stringstream s {R"(
    DisplayString ::=
      OCTET STRING
  )"};
  EXPECT_NO_THROW(p.parse(s));
}

TEST_F(Parser_test, long_data_type_no_range)
{
  Parser p;
  stringstream s {R"(
    TimeTicks ::=
      [APPLICATION 3]
        IMPLICIT INTEGER
  )"};
  EXPECT_NO_THROW(p.parse(s));
}

TEST_F(Parser_test, long_data_type)
{
  Parser p;
  stringstream s {R"(
    IpAddress ::=
      [APPLICATION 0]
        IMPLICIT OCTET STRING (SIZE (4))
  )"};
  EXPECT_NO_THROW(p.parse(s));
}

TEST_F(Parser_test, negative)
{
  Parser p;
  stringstream s {R"(tf sdohf kjdsf)"};
  EXPECT_THROW(p.parse(s), Exception);
}

TEST_F(Parser_test, skipping_comments)
{
  Parser p;
  stringstream s {R"(
    DisplayString ::= -- RANDOM COMMENT
      OCTET STRING
  )"};
  EXPECT_NO_THROW(p.parse(s));
}

TEST_F(Parser_test, rfc1213_mib_txt)
{
  Parser p;
  ifstream s {"resources/RFC1213-MIB.txt"};
  // EXPECT_NO_THROW(p.parse(s));
  p.parse(s);
}
