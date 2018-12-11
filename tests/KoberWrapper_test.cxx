#include "mpask/KoberWrapper.hxx"

#include "mpask/Parser.hxx"
#include "mpask/DataValue.hxx"
#include "mpask/DataSequence.hxx"

#include <gtest/gtest.h>
#include <iostream>
#include <memory>

using namespace std;
using namespace mpask;

class KoberWrapper_test : public ::testing::Test
{
};

TEST_F(KoberWrapper_test, single_value)
{
  stringstream s {R"(
    Abc DEFINITIONS ::= BEGIN
      Abc ::= [APPLICATION 1] IMPLICIT INTEGER
    END
  )"};
  auto schema = Parser{}(s);
  KoberWrapper kober {schema};

  auto data = make_shared<DataValue>();
  data->setType("INTEGER");
  data->setValue("123");
  
  auto code = kober.encode(data);
  decltype(code) golden = {};
  EXPECT_EQ(code, golden);
}

TEST_F(KoberWrapper_test, simple_sequence)
{
  stringstream s {R"(
    Abc DEFINITIONS ::= BEGIN
      Abc ::= [APPLICATION 1] IMPLICIT INTEGER
    END
  )"};
  auto schema = Parser{}(s);
  KoberWrapper kober {schema};

  auto data1 = make_shared<DataValue>();
  data1->setType("INTEGER");
  data1->setValue("221");

  auto data2 = make_shared<DataValue>();
  data2->setType("INTEGER");
  data2->setValue("238");

  auto sequence = make_shared<DataSequence>();
  sequence->append(data1);
  sequence->append(data2);
  
  auto code = kober.encode(sequence);
  decltype(code) golden = {};
  EXPECT_EQ(code, golden);
}
