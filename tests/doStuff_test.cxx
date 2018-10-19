#include "mibparser/doStuff.hxx"

#include <gtest/gtest.h>

using namespace mibparser;

class doStuff_test : public ::testing::Test
{
};

TEST_F(doStuff_test, example)
{
  EXPECT_EQ(doStuff(3), 1);
}
