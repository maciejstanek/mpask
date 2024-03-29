#include "mpask/MiniService.hxx"

#include <gtest/gtest.h>
#include <vector>

using namespace std;
using namespace mpask;

class MiniService_test : public ::testing::Test
{
};

TEST_F(MiniService_test, basic)
{
  vector<unsigned char> v = {
    0x30, 0x58, 0x02, 0x01, 0x01, 0x04, 0x07, 0x70, 0x72, 0x69, 0x76, 0x61,
    0x74, 0x65, 0xa0, 0x48, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01,
    0x00, 0x30, 0x3f, 0x30, 0x0c, 0x06, 0x07, 0x2b, 0x06, 0x01, 0x02, 0x01,
    0x04, 0x01, 0x82, 0x01, 0x01, 0x30, 0x1e, 0x06, 0x07, 0x2b, 0x06, 0x01,
    0x02, 0x01, 0x01, 0x05, 0x84, 0x13, 0x45, 0x78, 0x61, 0x6d, 0x70, 0x6c,
    0x65, 0x20, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x20, 0x4e, 0x61, 0x6d,
    0x65, 0x30, 0x0f, 0x06, 0x07, 0x2b, 0x06, 0x01, 0x02, 0x01, 0x01, 0x03,
    0x82, 0x04, 0x5c, 0x6d, 0xe7, 0x26,
  };
  auto r = MiniService().handleRequest(v);
  vector<unsigned char> golden = {
    0x30, 0x53, 0x02, 0x01, 0x01, 0x04, 0x07, 0x70, 0x72, 0x69, 0x76, 0x61,
    0x74, 0x65, 0xa2, 0x43, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x01,
    0x00, 0x30, 0x3a, 0x30, 0x0c, 0x06, 0x07, 0x2b, 0x06, 0x01, 0x02, 0x01,
    0x04, 0x01, 0x82, 0x01, 0x03, 0x30, 0x19, 0x06, 0x07, 0x2b, 0x06, 0x01,
    0x02, 0x01, 0x01, 0x05, 0x84, 0x0e, 0x6d, 0x61, 0x65, 0x6c, 0x73, 0x74,
    0x72, 0x6f, 0x6d, 0x2d, 0x78, 0x32, 0x32, 0x30, 0x30, 0x0f, 0x06, 0x07,
    0x2b, 0x06, 0x01, 0x02, 0x01, 0x01, 0x03, 0x82, 0x04, 0x5c, 0x6d, 0xe7,
    0x26,
  };
#if 1
  for (auto x : r) {
    cerr << "0x" << hex << setw(2) << setfill('0') << static_cast<int>(x) << ", ";
  }
  cerr << endl;
#endif
  EXPECT_EQ(r, golden);
}
