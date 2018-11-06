#include "mpask/TreeBuilder.hxx"

#include "mpask/Parser.hxx"
#include "mpask/Exception.hxx"

#include <gtest/gtest.h>
#include <sstream>
#include <fstream>

using namespace std;
using namespace mpask;

class TreeBuilder_test : public ::testing::Test
{
};

TEST_F(TreeBuilder_test, example_1)
{
  stringstream input {R"(
    x DEFINITIONS ::= BEGIN
      mib-2 OBJECT IDENTIFIER ::= { mgmt 1 }
      aaa OBJECT IDENTIFIER ::= { mib-2 1 }
      bbb OBJECT IDENTIFIER ::= { mib-2 2 }
      ccc OBJECT IDENTIFIER ::= { mib-2 3 }
      ddd OBJECT IDENTIFIER ::= { bbb 1 }
      eee OBJECT IDENTIFIER ::= { bbb 2 }
      fff OBJECT IDENTIFIER ::= { bbb 3 }
      ggg OBJECT IDENTIFIER ::= { eee 1 }
      hhh OBJECT IDENTIFIER ::= { eee 2 }
      iii OBJECT IDENTIFIER ::= { eee 3 }
    END
  )"};
  auto object = Parser{}(input);

  auto root = TreeBuilder{}(object);
  ASSERT_EQ(root->getName(), "mib-2"s); // NOTE: Always the root.
  EXPECT_EQ(root->size(), 3);
  int nodeIter {};
  shared_ptr<Node> subroot;
  for (const auto& [identifier, node] : *root) {
    switch (++nodeIter) {
      case 1:
        EXPECT_EQ(node->getName(), "aaa"s);
        EXPECT_EQ(node->getIdentifier(), 1);
        EXPECT_EQ(identifier, 1);
        EXPECT_EQ(node->size(), 0);
        break;
      case 2:
        EXPECT_EQ(node->getName(), "bbb"s);
        EXPECT_EQ(node->getIdentifier(), 2);
        EXPECT_EQ(identifier, 2);
        EXPECT_EQ(node->size(), 3);
        subroot = node;
        break;
      case 3:
        EXPECT_EQ(node->getName(), "ccc"s);
        EXPECT_EQ(node->getIdentifier(), 3);
        EXPECT_EQ(identifier, 3);
        EXPECT_EQ(node->size(), 0);
        break;
    }
  }
  EXPECT_EQ(nodeIter, 3);
  ASSERT_NE(subroot, nullptr);

  int subnodeIter {};
  for (const auto& [identifier, subnode] : *subroot) {
    switch (++subnodeIter) {
      case 1:
        EXPECT_EQ(subnode->getName(), "ddd"s);
        EXPECT_EQ(subnode->getIdentifier(), 1);
        EXPECT_EQ(identifier, 1);
        EXPECT_EQ(subnode->size(), 0);
        break;
      case 2:
        EXPECT_EQ(subnode->getName(), "eee"s);
        EXPECT_EQ(subnode->getIdentifier(), 2);
        EXPECT_EQ(identifier, 2);
        EXPECT_EQ(subnode->size(), 3);
        break;
      case 3:
        EXPECT_EQ(subnode->getName(), "fff"s);
        EXPECT_EQ(subnode->getIdentifier(), 3);
        EXPECT_EQ(identifier, 3);
        EXPECT_EQ(subnode->size(), 0);
        break;
    }
  }
  EXPECT_EQ(subnodeIter, 3);
}

TEST_F(TreeBuilder_test, mib)
{
  ifstream input {"resources/RFC1213-MIB.txt"};
  auto object = Parser{}(input);
  auto root = TreeBuilder{}(object);
  stringstream output;
  root->printHierarchy(0, output);
  cerr << output.str();
}
