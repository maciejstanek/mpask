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
  decltype(code) golden = {0x42, 0x01, 0x0f};
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
  decltype(code) golden = {0x30, 0x06, 0x42, 0x01, 0xdd, 0x82, 0x01, 0xee};
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
        Uvwuvw Uvw
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
  decltype(code) golden = {0x30, 0x0b, 0x02, 0x01, 0xee, 0x30, 0x06, 0x42, 0x01, 0xcc, 0x82, 0x01, 0xdd};
  EXPECT_EQ(code, golden);
}

TEST_F(Kober_test, long_sequence)
{
  stringstream s {R"(
    Abc DEFINITIONS ::= BEGIN
      Def ::= [APPLICATION 1] IMPLICIT INTEGER
      Ghi ::= SEQUENCE
      {
        Def00 Def,
        Def01 Def,
        Def02 Def,
        Def03 Def,
        Def04 Def,
        Def05 Def,
        Def06 Def,
        Def07 Def,
        Def010 Def,
        Def011 Def,
        Def012 Def,
        Def013 Def,
        Def014 Def,
        Def015 Def,
        Def016 Def,
        Def017 Def,
        Def020 Def,
        Def021 Def,
        Def022 Def,
        Def023 Def,
        Def024 Def,
        Def025 Def,
        Def026 Def,
        Def027 Def,
        Def030 Def,
        Def031 Def,
        Def032 Def,
        Def033 Def,
        Def034 Def,
        Def035 Def,
        Def036 Def,
        Def037 Def,
        Def040 Def,
        Def041 Def,
        Def042 Def
      }
    END
  )"};
  auto schema = Parser{}(s);
  Kober kober {schema};

  auto data = make_shared<DataValue>();
  data->setType("INTEGER");
  data->setValue("255");
  data->setContextAlias(schema->aliases.at(0)); // Assuming is 0th

  auto sequence = make_shared<DataSequence>();
  for (int i = 0; i < 43; ++i) {
    sequence->append(data);
  }
  sequence->setContextSequence(schema->sequences.at(0)); // Assuming Uvw is 0th

  auto code = kober.encode(sequence);
  decltype(code) golden = {0x30, 0x81, 0x81};
  for (int i = 0; i < 43; ++i) {
    golden.push_back(0x42);
    golden.push_back(0x01);
    golden.push_back(0xff);
  }
  EXPECT_EQ(code, golden);
}
