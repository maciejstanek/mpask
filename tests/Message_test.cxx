#include "mpask/Message.hxx"

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
};

TEST_F(Message_test, encode_basic)
{
  Message m {
    0,
    "private",
    MessageType::GetRequest,
    {
      {
        {"aaa", {{"bbb", 1}, {"ccc", 2}}, 3},
        "AAA" // "EXAMPLE VALUE"
      }
    }
  };

#if 0
  for (auto x : m.encode()) {
    cerr << "0x" << hex << setw(2) << setfill('0') << static_cast<int>(x) << ", ";
  }
  cerr << endl;
#endif

  vector<unsigned char> golden = {
    0x30, 0x24, 0x02, 0x01, 0x00, 0x04, 0x07, 0x70, 0x72, 0x69, 0x76, 0x61,
    0x74, 0x65, 0xa0, 0x14, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01,
    0x00, 0x30, 0x0b, 0x30, 0x09, 0x06, 0x02, 0x2a, 0x03, 0x04, 0x03, 0x41,
    0x41, 0x41
  };

  EXPECT_EQ(m.encode(), golden);
}
