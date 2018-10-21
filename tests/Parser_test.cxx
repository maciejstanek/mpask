#include "mpask/Parser.hxx"

#include <gtest/gtest.h>
#include <sstream>

using namespace std;
using namespace mpask;

class Parser_test : public ::testing::Test
{
};

TEST_F(Parser_test, example)
{
  Parser p;
  stringstream s {R"(
    system     OBJECT IDENTIFIER ::= { mib-2 1 }
    interfaces OBJECT IDENTIFIER ::= { mib-2 2 }
    at         OBJECT IDENTIFIER ::= { mib-2 3 }
    ip         OBJECT IDENTIFIER ::= { mib-2 4 }
    icmp       OBJECT IDENTIFIER ::= { mib-2 5 }
    tcp        OBJECT IDENTIFIER ::= { mib-2 6 }
  )"};
  p.parse(s);
}
