#include "mpask/MasterParser.hxx"

#include <gtest/gtest.h>
#include <fstream>

using namespace std;
using namespace mpask;

class MasterParser_test : public ::testing::Test
{
};

TEST_F(MasterParser_test, mib_a_b)
{
  auto result = MasterParser{}("resources", "a");
}

TEST_F(MasterParser_test, mib_full)
{
  auto result = MasterParser{}("resources", "RFC1213-MIB");
}
