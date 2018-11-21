#include "mpask/Node.hxx"

#include "mpask/Exception.hxx"
#include "mpask/TypeDeclaration.hxx"
#include "mpask/TreeBuilder.hxx"
#include "mpask/MasterParser.hxx"

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>

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
  n->printHierarchy(s);
  EXPECT_EQ(s.str(),
    "abc()\n"
    "  def(1)\n"
    "  ghi(2)\n"
    "    jkl(2.1)\n"
    "    mno(2.2)\n"
  );
  stringstream s2;   
  n->printHierarchy(s2, "-  ");
  EXPECT_EQ(s2.str(),
    "abc()\n"
    "-  def(1)\n"
    "-  ghi(2)\n"
    "-  -  jkl(2.1)\n"
    "-  -  mno(2.2)\n"
  );
  stringstream d;   
  n->printDotFile(d);
  EXPECT_EQ(d.str(),
		"digraph mib {\n"
		"  n [shape=\"record\",margin=\"0.1\",width=\"0\",height=\"0\",label=\"{<name>abc|<oid>}\"];\n"
		"  n_1 [shape=\"record\",margin=\"0.1\",width=\"0\",height=\"0\",label=\"{<name>def|<oid>1}\"];\n"
		"  n_2 [shape=\"record\",margin=\"0.1\",width=\"0\",height=\"0\",label=\"{<name>ghi|<oid>2}\"];\n"
		"  n_2_1 [shape=\"record\",margin=\"0.1\",width=\"0\",height=\"0\",label=\"{<name>jkl|<oid>2.1}\"];\n"
		"  n_2_2 [shape=\"record\",margin=\"0.1\",width=\"0\",height=\"0\",label=\"{<name>mno|<oid>2.2}\"];\n"
		"\n"
		"  n:oid -> n_1:name;\n"
		"  n:oid -> n_2:name;\n"
		"  n_2:oid -> n_2_1:name;\n"
		"  n_2:oid -> n_2_2:name;\n"
		"}\n"
	);
  ofstream dot;
  dot.open("node_test.dot");
  n->printDotFile(dot);
  dot.close();
}

TEST_F(Node_test, oid_conversions)
{
  Node n {{}, {}};
  vector<int> oidVector {1, 2, 3, 4};
  string oidString {"1.2.3.4"};
  EXPECT_EQ(n.oidToString(oidVector), oidString);
  EXPECT_EQ(n.stringToOid(oidString), oidVector);
  EXPECT_THROW(n.stringToOid("1.abc.3.4"), std::invalid_argument); // FIXME: Should handle this as a node name. Now throwing an exception.
}

TEST_F(Node_test, get_by_oid)
{
  auto object = MasterParser{}("resources", "simple");
  auto root = TreeBuilder{}(object);

  try {
    root->findNodeByOID(vector {1, 9, 12, 1});
    FAIL() << "should throw";
  }
  catch (const Exception &e) {
    EXPECT_EQ(e.what(), "Could not find OID '1.9.12.1'. Failed on searching for '1.9.12'."s);
  }

  shared_ptr<Node> node;
  ASSERT_NO_THROW(node = root->findNodeByOID(vector {1, 9, 2, 3}));
  EXPECT_EQ(node->getName(), "fff"s);

  shared_ptr<Node> node2;
  ASSERT_NO_THROW(node2 = root->findNodeByOID("1.9.2.3"));
  EXPECT_EQ(node2->getName(), "fff"s);
}

TEST_F(Node_test, get_by_name)
{
  auto object = MasterParser{}("resources", "simple");
  auto root = TreeBuilder{}(object);

  vector<int> result;
  ASSERT_NO_THROW(result = root->findOidByName("fff"));
  ASSERT_EQ(result.size(), 4);
  EXPECT_EQ(result[0], 1);
  EXPECT_EQ(result[1], 9);
  EXPECT_EQ(result[2], 2);
  EXPECT_EQ(result[3], 3);
}
