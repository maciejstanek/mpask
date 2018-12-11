#include "mpask/Kober.hxx"

#include "mpask/Parser.hxx"
#include "mpask/DataValue.hxx"
#include "mpask/DataSequence.hxx"

#include <gtest/gtest.h>
#include <iostream>
#include <memory>

using namespace std;
using namespace mpask;

class Kober_test : public ::testing::Test
{
};

TEST_F(Kober_test, single_value)
{
  stringstream s {R"(
    Abc DEFINITIONS ::= BEGIN
      Abc ::= [APPLICATION 3] IMPLICIT INTEGER
    END
  )"};
  auto schema = Parser{}(s);
  Kober kober {schema};

  auto data = make_shared<DataValue>();
  data->setType("INTEGER");
  data->setValue("15");
  data->setContextAlias(schema->aliases.at(0));
  
  auto code = kober.encode(data);
  decltype(code) golden = {};
  EXPECT_EQ(code, golden);
}

TEST_F(Kober_test, simple_sequence)
{
  stringstream s {R"(
    Abc DEFINITIONS ::= BEGIN
      Abc ::= [APPLICATION 1] IMPLICIT INTEGER
      Def ::= [CONTEXT-SPECIFIC 2] EXPLICIT INTEGER
      Xyz ::= SEQUENCE
      {
        Abcabc Abc,
        Defdef Def
      }
    END
  )"};
  auto schema = Parser{}(s);
  Kober kober {schema};

  auto data1 = make_shared<DataValue>();
  data1->setType("INTEGER");
  data1->setValue("221");
  data1->setContextAlias(schema->aliases.at(0)); // Assuming Abc is 0th

  auto data2 = make_shared<DataValue>();
  data2->setType("INTEGER");
  data2->setValue("238");
  data2->setContextAlias(schema->aliases.at(1)); // Assuming Def is 1th

  auto sequence = make_shared<DataSequence>();
  sequence->append(data1);
  sequence->append(data2);
  sequence->setContextSequence(schema->sequences.at(0));
  
  auto code = kober.encode(sequence);
  decltype(code) golden = {};
  EXPECT_EQ(code, golden);
}

TEST_F(Kober_test, nested_sequence)
{
  stringstream s {R"(
    Abc DEFINITIONS ::= BEGIN
      Abc ::= [APPLICATION 1] IMPLICIT INTEGER
      Def ::= [CONTEXT-SPECIFIC 2] EXPLICIT INTEGER
      Ghi ::= [UNIVERSAL 3] EXPLICIT INTEGER
      Uvw ::= SEQUENCE
      {
        Abcabc Abc,
        Defdef Def
      }
      Xyz ::= SEQUENCE
      {
        Ghighi Ghi,
        Xyzxyz Xyz
      }
    END
  )"};
  auto schema = Parser{}(s);
  Kober kober {schema};

  auto data1 = make_shared<DataValue>();
  data1->setType("INTEGER");
  data1->setValue("204");
  data1->setContextAlias(schema->aliases.at(0)); // Assuming Abc is 0th

  auto data2 = make_shared<DataValue>();
  data2->setType("INTEGER");
  data2->setValue("221");
  data2->setContextAlias(schema->aliases.at(1)); // Assuming Def is 1st

  auto data3 = make_shared<DataValue>();
  data3->setType("INTEGER");
  data3->setValue("238");
  data3->setContextAlias(schema->aliases.at(2)); // Assuming Ghi is 2nd

  auto sequence1 = make_shared<DataSequence>();
  sequence1->append(data1);
  sequence1->append(data2);
  sequence1->setContextSequence(schema->sequences.at(0)); // Assuming Uvw is 0th

  auto sequence2 = make_shared<DataSequence>();
  sequence2->append(data3);
  sequence2->append(sequence1);
  sequence2->setContextSequence(schema->sequences.at(1)); // Assuming Xyz is 1st
  
  auto code = kober.encode(sequence2);
  decltype(code) golden = {};
  EXPECT_EQ(code, golden);
}
