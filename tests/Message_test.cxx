#include "mpask/Message.hxx"

#include "mpask/Parser.hxx"
#include <gtest/gtest.h>

using namespace std;
using namespace mpask;

bool operator==(const vector<unsigned char>& lhs, const vector<unsigned char>& rhs)
{
  if (lhs.size() != rhs.size()) {
    return false;
  }
  for (auto li = lhs.begin(), ri = rhs.begin(); li != lhs.end(); ++ri, ++li) {
    if (*li != *ri) {
      return false;
    }
  }
  return true;
}

class Message_test : public ::testing::Test
{
  protected:
    shared_ptr<MIBFile> generateSimpleSchema()
    {
      stringstream s {R"(
        testenv DEFINITIONS ::= BEGIN
          sub OBJECT IDENTIFIER ::= { iso 2 }
          int OBJECT-TYPE SYNTAX INTEGER ::= { sub 3 }
          string OBJECT-TYPE SYNTAX OCTET STRING ::= { sub 4 }
        END
      )"};
      return Parser{}(s);
    }
};

TEST_F(Message_test, encode_basic)
{
  Message m {
    {{{1, 2, 3}, "170"}, {{1, 2, 4}, "AAA"}},
    generateSimpleSchema()
  };

#if 0
  for (auto x : m.encode()) {
    cerr << "0x" << hex << setw(2) << setfill('0') << static_cast<int>(x) << ", ";
  }
  cerr << endl;
#endif

  vector<unsigned char> golden = {
    0x30, 0x2d, 0x02, 0x01, 0x01, 0x04, 0x07, 0x70, 0x72, 0x69, 0x76, 0x61,
    0x74, 0x65, 0xa0, 0x1d, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01,
    0x00, 0x30, 0x14, 0x30, 0x07, 0x06, 0x02, 0x2a, 0x03, 0x82, 0x01, 0xaa,
    0x30, 0x09, 0x06, 0x02, 0x2a, 0x04, 0x84, 0x03, 0x41, 0x41, 0x41,
  };

  EXPECT_EQ(m.encode(), golden);
}

TEST_F(Message_test, decode_basic)
{
  vector<unsigned char> code = {
    0x30, 0x2d, 0x02, 0x01, 0x01, 0x04, 0x07, 0x70, 0x72, 0x69, 0x76, 0x61,
    0x74, 0x65, 0xa0, 0x1d, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01,
    0x00, 0x30, 0x14, 0x30, 0x07, 0x06, 0x02, 0x2a, 0x03, 0x82, 0x01, 0xaa,
    0x30, 0x09, 0x06, 0x02, 0x2a, 0x04, 0x84, 0x03, 0x41, 0x41, 0x41,
  };
  Message decoded {
    code,
    generateSimpleSchema()
  };

  Message golden {
    {{{1, 2, 3}, "170"}, {{1, 2, 4}, "AAA"}},
    generateSimpleSchema()
  };

  EXPECT_EQ(decoded, golden);
  EXPECT_EQ(decoded.version, golden.version);
  EXPECT_EQ(decoded.community, golden.community);
  EXPECT_EQ(decoded.type, golden.type);
  ASSERT_EQ(decoded.values.size(), 2u);
  // TODO: Test in detail
  EXPECT_EQ(decoded.values, golden.values);
}
