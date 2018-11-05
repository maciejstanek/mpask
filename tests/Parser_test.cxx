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

TEST_F(Parser_test, multiple_statements)
{
  stringstream s {R"(
    x DEFINITIONS ::= BEGIN
      system     OBJECT IDENTIFIER ::= { mib-2 1 }
      interfaces OBJECT IDENTIFIER ::= { mib-2 2 }
      at         OBJECT IDENTIFIER ::= { mib-2 3 }
    END
  )"};
  EXPECT_NO_THROW(Parser{}(s));
}

TEST_F(Parser_test, simple_object_identifier)
{
  stringstream s {R"(
    x DEFINITIONS ::= BEGIN
      system OBJECT IDENTIFIER ::= { mib-2 1 }
    END
  )"};
  EXPECT_NO_THROW(Parser{}(s));
}

TEST_F(Parser_test, long_object_identifier)
{
  stringstream s {R"(
    x DEFINITIONS ::= BEGIN
      internet OBJECT IDENTIFIER ::= { iso org(3) dod(6) 1 }
    END
  )"};
  EXPECT_NO_THROW(Parser{}(s));
}

TEST_F(Parser_test, simple_object_type)
{
  stringstream s {R"(
    x DEFINITIONS ::= BEGIN
      sysUpTime OBJECT-TYPE
        ::= { system 3 }
    END
  )"};
  EXPECT_NO_THROW(Parser{}(s));
}

TEST_F(Parser_test, long_object_type)
{
  stringstream s {R"(
    x DEFINITIONS ::= BEGIN
      sysUpTime OBJECT-TYPE
        SYNTAX TimeTicks
        ACCESS read-only
        STATUS mandatory
        DESCRIPTION
          "The time (in hundredths of a second) since the
          network management portion of the system was last
          re-initialized."
        ::= { system 3 }
    END
  )"};
  EXPECT_NO_THROW(Parser{}(s));
}

TEST_F(Parser_test, simple_data_type)
{
  stringstream s {R"(
    x DEFINITIONS ::= BEGIN
      DisplayString ::=
        OCTET STRING
    END
  )"};
  EXPECT_NO_THROW(Parser{}(s));
}

TEST_F(Parser_test, long_data_type_no_range)
{
  stringstream s {R"(
    x DEFINITIONS ::= BEGIN
      TimeTicks ::=
        [APPLICATION 3]
          IMPLICIT INTEGER
    END
  )"};
  EXPECT_NO_THROW(Parser{}(s));
}

TEST_F(Parser_test, long_data_type)
{
  stringstream s {R"(
    x DEFINITIONS ::= BEGIN
      IpAddress ::=
        [APPLICATION 0]
          IMPLICIT OCTET STRING (SIZE (4))
    END
  )"};
  EXPECT_NO_THROW(Parser{}(s));
}

TEST_F(Parser_test, negative)
{
  stringstream s {R"(tf sdohf kjdsf)"};
  EXPECT_THROW(Parser{}(s), Exception);
}

TEST_F(Parser_test, skipping_comments)
{
  stringstream s {R"(
    x DEFINITIONS ::= BEGIN
      DisplayString ::= -- RANDOM COMMENT
        OCTET STRING
    END
  )"};
  EXPECT_NO_THROW(Parser{}(s));
}

TEST_F(Parser_test, rfc1213_mib_txt)
{
  ifstream s {"resources/RFC1213-MIB.txt"};
  EXPECT_NO_THROW(Parser{}(s));
}
