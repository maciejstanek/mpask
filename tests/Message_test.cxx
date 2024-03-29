#include "mpask/Message.hxx"

#include "mpask/MasterParser.hxx"
#include "mpask/Parser.hxx"
#include "mpask/Exception.hxx"
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
          int OBJECT-TYPE SYNTAX INTEGER (10..20) ::= { sub 3 }
          string OBJECT-TYPE SYNTAX OCTET STRING (SIZE (10..20)) ::= { sub 4 }
        END
      )"};
      return Parser{}(s);
    }
    shared_ptr<MIBFile> generateMibSchema()
    {
      return MasterParser{}("resources", "RFC1213-MIB");
    }
};

TEST_F(Message_test, encode_basic)
{
  Message m {
    {{{1, 2, 3}, "170"}, {{1, 2, 4}, "AAA"}},
    generateSimpleSchema()
  };
  m.type = MessageType::SetRequest;

#if 0
  for (auto x : m.encode()) {
    cerr << "0x" << hex << setw(2) << setfill('0') << static_cast<int>(x) << ", ";
  }
  cerr << endl;
#endif

  vector<unsigned char> golden = {
    0x30, 0x2d, 0x02, 0x01, 0x01, 0x04, 0x07, 0x70, 0x72, 0x69, 0x76, 0x61,
    0x74, 0x65, 0xa3, 0x1d, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01,
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

  // TEST VALIDATION
}

TEST_F(Message_test, validate_int_range)
{
  Message m {
    {{{1, 2, 3}, "170"}},
    generateSimpleSchema()
  };
  try {
    m.validate();
    FAIL() << "should throw";
  }
  catch (Exception& e) {
    EXPECT_EQ(e.what(), "Object 1.2.3. of type INTEGER does not fit in the allowed range of values (170 is not in <10, 20>)."s);
  }
}

TEST_F(Message_test, validate_string_size)
{
  Message m {
    {{{1, 2, 4}, "AAA"}},
    generateSimpleSchema()
  };
  try {
    m.validate();
    FAIL() << "should throw";
  }
  catch (Exception& e) {
    EXPECT_EQ(e.what(), "Object 1.2.4. of type OCTET STRING does not fit in the allowed size (3 is not in <10, 20>)."s);
  }
}

TEST_F(Message_test, encode_mib_simple)
{
  Message m {
    {{{1,3,6,1,2,1,4,1}, "1"}},
    generateMibSchema()
  };

#if 0
  for (auto x : m.encode()) {
    cerr << "0x" << hex << setw(2) << setfill('0') << static_cast<int>(x) << ", ";
  }
  cerr << endl;
#endif

  vector<unsigned char> golden = {
    0x30, 0x27, 0x02, 0x01, 0x01, 0x04, 0x07, 0x70, 0x72, 0x69, 0x76, 0x61,
    0x74, 0x65, 0xa0, 0x17, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01,
    0x00, 0x30, 0x0e, 0x30, 0x0c, 0x06, 0x07, 0x2b, 0x06, 0x01, 0x02, 0x01,
    0x04, 0x01, 0x82, 0x01, 0x01,
  };
  EXPECT_EQ(m.encode(), golden);
}

TEST_F(Message_test, decode_mib_simple)
{
  vector<unsigned char> code = {
    0x30, 0x27, 0x02, 0x01, 0x01, 0x04, 0x07, 0x70, 0x72, 0x69, 0x76, 0x61,
    0x74, 0x65, 0xa0, 0x17, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01,
    0x00, 0x30, 0x0e, 0x30, 0x0c, 0x06, 0x07, 0x2b, 0x06, 0x01, 0x02, 0x01,
    0x04, 0x01, 0x82, 0x01, 0x01,
  };
  Message decoded {
    code,
    generateMibSchema()
  };

  Message golden {
    {{{1,3,6,1,2,1,4,1}, "1"}},
    generateMibSchema()
  };

  EXPECT_EQ(decoded, golden);
  EXPECT_EQ(decoded.version, golden.version);
  EXPECT_EQ(decoded.community, golden.community);
  EXPECT_EQ(decoded.type, golden.type);
  ASSERT_EQ(decoded.values.size(), 1u);
  EXPECT_EQ(decoded.values, golden.values);
}

TEST_F(Message_test, encode_mib_advanced)
{
  Message m {
    {
      {{1,3,6,1,2,1,4,1}, "1"},
      {{1,3,6,1,2,1,1,5}, "Example System Name"},
      {{1,3,6,1,2,1,1,3}, "1550706470"}
    },
    generateMibSchema()
  };

#if 0
  for (auto x : m.encode()) {
    cerr << "0x" << hex << setw(2) << setfill('0') << static_cast<int>(x) << ", ";
  }
  cerr << endl;
#endif

  vector<unsigned char> golden = {
    0x30, 0x58, 0x02, 0x01, 0x01, 0x04, 0x07, 0x70, 0x72, 0x69, 0x76, 0x61,
    0x74, 0x65, 0xa0, 0x48, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01,
    0x00, 0x30, 0x3f, 0x30, 0x0c, 0x06, 0x07, 0x2b, 0x06, 0x01, 0x02, 0x01,
    0x04, 0x01, 0x82, 0x01, 0x01, 0x30, 0x1e, 0x06, 0x07, 0x2b, 0x06, 0x01,
    0x02, 0x01, 0x01, 0x05, 0x84, 0x13, 0x45, 0x78, 0x61, 0x6d, 0x70, 0x6c,
    0x65, 0x20, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x20, 0x4e, 0x61, 0x6d,
    0x65, 0x30, 0x0f, 0x06, 0x07, 0x2b, 0x06, 0x01, 0x02, 0x01, 0x01, 0x03,
    0x82, 0x04, 0x5c, 0x6d, 0xe7, 0x26,
  };
  EXPECT_EQ(m.encode(), golden);
}

TEST_F(Message_test, decode_mib_advanced)
{
  vector<unsigned char> code = {
    0x30, 0x58, 0x02, 0x01, 0x01, 0x04, 0x07, 0x70, 0x72, 0x69, 0x76, 0x61,
    0x74, 0x65, 0xa0, 0x48, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01,
    0x00, 0x30, 0x3f, 0x30, 0x0c, 0x06, 0x07, 0x2b, 0x06, 0x01, 0x02, 0x01,
    0x04, 0x01, 0x82, 0x01, 0x01, 0x30, 0x1e, 0x06, 0x07, 0x2b, 0x06, 0x01,
    0x02, 0x01, 0x01, 0x05, 0x84, 0x13, 0x45, 0x78, 0x61, 0x6d, 0x70, 0x6c,
    0x65, 0x20, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x20, 0x4e, 0x61, 0x6d,
    0x65, 0x30, 0x0f, 0x06, 0x07, 0x2b, 0x06, 0x01, 0x02, 0x01, 0x01, 0x03,
    0x82, 0x04, 0x5c, 0x6d, 0xe7, 0x26,
  };
  Message decoded {
    code,
    generateMibSchema()
  };

  Message golden {
    {
      {{1,3,6,1,2,1,4,1}, "1"},
      {{1,3,6,1,2,1,1,5}, "Example System Name"},
      {{1,3,6,1,2,1,1,3}, "1550706470"}
    },
    generateMibSchema()
  };

  EXPECT_EQ(decoded, golden);
  EXPECT_EQ(decoded.version, golden.version);
  EXPECT_EQ(decoded.community, golden.community);
  EXPECT_EQ(decoded.type, golden.type);
  ASSERT_EQ(decoded.values.size(), 3u);
  EXPECT_EQ(decoded.values, golden.values);
}
