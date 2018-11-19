#include "mpask/TreeBuilder.hxx"

#include "mpask/Parser.hxx"
#include "mpask/MasterParser.hxx"
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
      xxx OBJECT IDENTIFIER ::= { iso 123 }
      aaa OBJECT IDENTIFIER ::= { xxx 1 }
      bbb OBJECT IDENTIFIER ::= { xxx 2 }
      ccc OBJECT IDENTIFIER ::= { xxx 3 }
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

  stringstream output;
  root->printHierarchy(output);
  EXPECT_EQ(output.str(),
		"root()\n"
		"  iso(1)\n"
		"    xxx(1.123)\n"
		"      aaa(1.123.1)\n"
		"      bbb(1.123.2)\n"
		"        ddd(1.123.2.1)\n"
		"        eee(1.123.2.2)\n"
		"          ggg(1.123.2.2.1)\n"
		"          hhh(1.123.2.2.2)\n"
		"          iii(1.123.2.2.3)\n"
		"        fff(1.123.2.3)\n"
		"      ccc(1.123.3)\n"
	);

  ASSERT_EQ(root->getName(), "root"s); // NOTE: Always "root".
  ASSERT_EQ(root->size(), 1);
  auto& iso = root->begin()->second;
  ASSERT_EQ(iso->getName(), "iso"s);
  ASSERT_EQ(iso->size(), 1);
  auto& xxx = iso->begin()->second;
  ASSERT_EQ(xxx->getName(), "xxx"s);
  ASSERT_EQ(xxx->size(), 3);
  int nodeIter {};
  shared_ptr<Node> sub;
  for (const auto& [identifier, node] : *xxx) {
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
        sub = node;
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
  ASSERT_NE(sub, nullptr);

  int subsubIter {};
  for (const auto& [identifier, subsub] : *sub) {
    switch (++subsubIter) {
      case 1:
        EXPECT_EQ(subsub->getName(), "ddd"s);
        EXPECT_EQ(subsub->getIdentifier(), 1);
        EXPECT_EQ(identifier, 1);
        EXPECT_EQ(subsub->size(), 0);
        break;
      case 2:
        EXPECT_EQ(subsub->getName(), "eee"s);
        EXPECT_EQ(subsub->getIdentifier(), 2);
        EXPECT_EQ(identifier, 2);
        EXPECT_EQ(subsub->size(), 3);
        break;
      case 3:
        EXPECT_EQ(subsub->getName(), "fff"s);
        EXPECT_EQ(subsub->getIdentifier(), 3);
        EXPECT_EQ(identifier, 3);
        EXPECT_EQ(subsub->size(), 0);
        break;
    }
  }
  EXPECT_EQ(subsubIter, 3);
}

TEST_F(TreeBuilder_test, mib)
{
  auto object = MasterParser{}("resources", "RFC1213-MIB");
  auto root = TreeBuilder{}(object);
  stringstream output;
  root->printHierarchy(output);
  cerr << output.str();
  // TODO: Add some auto check.
  ofstream dot;
  dot.open("mib.dot");
  root->printDotFile(dot);
  dot.close();
}
