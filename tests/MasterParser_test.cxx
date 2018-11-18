#include "mpask/MasterParser.hxx"

#include "mpask/Exception.hxx"

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
  EXPECT_NE(result, nullptr);
  EXPECT_EQ(result->aliases.size(), 1);
  EXPECT_EQ(result->types.size(), 1);
  EXPECT_EQ(result->sequences.size(), 0);
}

TEST_F(MasterParser_test, mib_full)
{
  auto result = MasterParser{}("resources", "RFC1213-MIB");
  EXPECT_NE(result, nullptr);
  EXPECT_EQ(result->aliases.size(), 8);
  EXPECT_EQ(result->types.size(), 207);
  EXPECT_EQ(result->sequences.size(), 12);
}

TEST_F(MasterParser_test, import_error)
{
  try {
    MasterParser{}("resources", "import-error");
    FAIL() << "no exception thrown";
  }
  catch (Exception &e) {
    EXPECT_EQ(e.what(), "neither resources/non-existient-mib.mib nor resources/non-existient-mib.txt found"s);
  }
}
