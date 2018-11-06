#include "mpask/Node.hxx"

#include "mpask/Exception.hxx"
#include "mpask/TypeDeclaration.hxx"

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <sstream>

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
  n->addChild(456, n1);
  n->addChild(789, n2);
  EXPECT_EQ(n->size(), 2);
  int count {};
  for (const auto& [identifier, child] : *n) {
    switch (count++) {
      case 0:
        EXPECT_EQ(child->getName(), "def"s);
        EXPECT_EQ(child->getIdentifier(), 456);
        EXPECT_EQ(identifier, 456);
        break;
      case 1:
        EXPECT_EQ(child->getName(), "ghi"s);
        EXPECT_EQ(child->getIdentifier(), 789);
        EXPECT_EQ(identifier, 789);
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
  n2->addChild(1, n21);
  n2->addChild(2, n22);
  n->addChild(1, n1);
  n->addChild(2, n2);
  EXPECT_EQ(n1, n->findNodeByName("def"));
  EXPECT_EQ(n2, n->findNodeByName("ghi"));
  EXPECT_EQ(n21, n->findNodeByName("jkl"));
  EXPECT_EQ(n22, n->findNodeByName("mno"));
  stringstream s;   
  n->printHierarchy(0, s);
  EXPECT_EQ(s.str(),
    "abc(1)\n"
    "  def(1)\n"
    "  ghi(2)\n"
    "    jkl(1)\n"
    "    mno(2)\n"
  );
}
