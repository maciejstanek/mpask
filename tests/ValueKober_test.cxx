#include "mpask/ValueKober.hxx"

#include "mpask/AliasDeclarationGrammar.hxx"
#include "mpask/DataValue.hxx"

#include <boost/spirit/include/qi.hpp>
#include <gtest/gtest.h>
#include <string>
#include <map>
#include <iostream>
#include <memory>

using namespace std;
using namespace mpask;

class ValueKober_test : public ::testing::Test
{
protected:
  pair<bool, AliasDeclaration> parse(const string& input)
  {
    string::const_iterator iter = input.begin();
    string::const_iterator end = input.end();
    AliasDeclarationGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    AliasDeclaration result;
    bool status = phrase_parse(iter, end, parser, space, result);
    return {status, result};
  }
};

TEST_F(ValueKober_test, octet_string)
{
  string input {"abc ::= OCTET STRING"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto data = make_shared<DataValue>();
  data->setContextAlias(result);
  data->setValue("abcd");
  auto coded = ValueKober()(data);
  vector<unsigned char> golden {0x84, 0x04, 0x61, 0x62, 0x63, 0x64};
  ASSERT_EQ(coded, golden);
}

TEST_F(ValueKober_test, integer)
{
  string input {"abc ::= [UNIVERSAL 1] INTEGER"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto data = make_shared<DataValue>();
  data->setContextAlias(result);
  data->setValue("1234");
  auto coded = ValueKober()(data);
  vector<unsigned char> golden {0x02, 0x02, 0x04, 0xd2 };
  ASSERT_EQ(coded, golden);
}

TEST_F(ValueKober_test, integer_small)
{
  string input {"abc ::= [APPLICATION 1] INTEGER"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto data = make_shared<DataValue>();
  data->setContextAlias(result);
  data->setValue("7");
  auto coded = ValueKober()(data);
  vector<unsigned char> golden {0x42, 0x01, 0x07};
  ASSERT_EQ(coded, golden);
}

TEST_F(ValueKober_test, integer_big)
{
  string input {"abc ::= [CONTEXT-SPECIFIC 1] INTEGER"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto data = make_shared<DataValue>();
  data->setContextAlias(result);
  data->setValue("4294967295"); // 2^32-1
  auto coded = ValueKober()(data);
  vector<unsigned char> golden {0x82, 0x04, 0xff, 0xff, 0xff, 0xff};
  ASSERT_EQ(coded, golden);
}

TEST_F(ValueKober_test, integer_zero)
{
  string input {"abc ::= [PRIVATE 1] INTEGER"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto data = make_shared<DataValue>();
  data->setContextAlias(result);
  data->setValue("0");
  auto coded = ValueKober()(data);
  vector<unsigned char> golden {0xc2, 0x00};
  ASSERT_EQ(coded, golden);
}

TEST_F(ValueKober_test, integer_range)
{
  string input {"abc ::= [PRIVATE 1] INTEGER (0..100000)"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto data = make_shared<DataValue>();
  data->setContextAlias(result);
  data->setValue("63");
  auto coded = ValueKober()(data);
  vector<unsigned char> golden {0xc2, 0x01, 0x3f};
  ASSERT_EQ(coded, golden);
}

TEST_F(ValueKober_test, null)
{
  string input {"abc ::= [APPLICATION 2] NULL"};
  auto [status, result] = parse(input);
  ASSERT_EQ(status, true);
  auto data = make_shared<DataValue>();
  data->setContextAlias(result);
  data->setValue("");
  auto coded = ValueKober()(data);
  vector<unsigned char> golden {0x45, 0x00};
  ASSERT_EQ(coded, golden);
}

TEST_F(ValueKober_test, calculate_required_number_of_bits)
{
  ASSERT_EQ(calculateRequiredNumberOfBits(-1), 2);
  ASSERT_EQ(calculateRequiredNumberOfBits(0), 0);
  ASSERT_EQ(calculateRequiredNumberOfBits(1), 1);
  ASSERT_EQ(calculateRequiredNumberOfBits(254), 8);
  ASSERT_EQ(calculateRequiredNumberOfBits(255), 8);
  ASSERT_EQ(calculateRequiredNumberOfBits(256), 9);
  ASSERT_EQ(calculateRequiredNumberOfBits(257), 9);
  ASSERT_EQ(calculateRequiredNumberOfBits(-254), 9);
  ASSERT_EQ(calculateRequiredNumberOfBits(-255), 9);
  ASSERT_EQ(calculateRequiredNumberOfBits(-256), 10);
  ASSERT_EQ(calculateRequiredNumberOfBits(-257), 10);
}
