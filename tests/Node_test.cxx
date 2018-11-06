#include "mpask/Node.hxx"

#include "mpask/Exception.hxx"
#include "mpask/TypeDeclaration.hxx"

#include <gtest/gtest.h>
#include <memory>
#include <string>

using namespace std;
using namespace mpask;

class Node_test : public ::testing::Test
{
};

TEST_F(Node_test, getters)
{
  auto n = make_shared<Node>("abc", 123);
  EXPECT_EQ(n->getName(), "abc"s);
  EXPECT_EQ(n->getIdentifier(), 123);
}

TEST_F(Node_test, source)
{
  auto n = make_shared<Node>("B", 13);
  auto t = make_shared<TypeDeclaration>();
  t->name = "qwerty";
  n->setSource(*t); // FIXME: SHOULD pass by ref.
  EXPECT_EQ(n->getSource().name, "qwerty"s);
  // EXPECT_EQ(n->getSource(), *t);
}

TEST_F(Node_test, iterator)
{
  auto n = make_shared<Node>("abc", 123);
  auto n1 = make_shared<Node>("def", 456);
  auto n2 = make_shared<Node>("ghi", 789);
  n->addChild(n1);
  n->addChild(n2);
  EXPECT_EQ(n->size(), 2);
  int count {};
  for (const auto& child : *n) {
    switch (count++) {
      case 0:
        EXPECT_EQ(child->getName(), "def"s);
        EXPECT_EQ(child->getIdentifier(), 456);
        break;
      case 1:
        EXPECT_EQ(child->getName(), "ghi"s);
        EXPECT_EQ(child->getIdentifier(), 789);
        break;
    }
  }
  EXPECT_EQ(count, 2);
}

TEST_F(Node_test, find_by_name)
{
  auto n = make_shared<Node>("abc", 1);
  auto n1 = make_shared<Node>("def", 1);
  auto n2 = make_shared<Node>("ghi", 2);
  auto n21 = make_shared<Node>("jkl", 1);
  auto n22 = make_shared<Node>("mno", 2);
  n2->addChild(n21);
  n2->addChild(n22);
  n->addChild(n1);
  n->addChild(n2);
  EXPECT_EQ(n1, n->findNodeByName("def"));
  EXPECT_EQ(n2, n->findNodeByName("ghi"));
  EXPECT_EQ(n21, n->findNodeByName("jkl"));
  EXPECT_EQ(n22, n->findNodeByName("mno"));
}
