#include "mpask/Node.hxx"

#include "mpask/Exception.hxx"

#include <iostream>

using namespace std;

namespace mpask
{
  Node::Node(const std::string& newName, int newIdentifier)
    : name {newName},
      identifier {newIdentifier}
  {
  }

  void Node::addChild(int nodeIdentifier, const std::shared_ptr<Node>& node)
  {
    children[nodeIdentifier] = node;
  }

  shared_ptr<Node> Node::findNodeByName(const std::string& neddle) const
  {
    for (const auto& child : children) {
      if (child.second->getName() == neddle) {
        return child.second;
      }
      if (auto result = child.second->findNodeByName(neddle)) {
        return result;
      }
    }
    shared_ptr<Node> empty;
    return empty;
  }

  std::string Node::getName() const
  {
    return name;
  }

  int Node::getIdentifier() const
  {
    return identifier;
  }

  Node::Iterator Node::begin()
  {
    return children.begin();
  }

  Node::Iterator Node::end()
  {
    return children.end();
  }

  void Node::setSource(const TypeDeclaration& newSource)
  {
    source = newSource;
  }

  const TypeDeclaration& Node::getSource() const
  {
    return source;
  }

  int Node::size() const
  {
    return static_cast<int>(children.size());
  }

  void Node::printHierarchy(int depth, ostream& output) const
  {
    auto padding = string(depth * 2, ' ');
    output << padding << getName() << "("  << getIdentifier() << ")" << endl;
    for (const auto& child : children) {
      child.second->printHierarchy(depth + 1, output);
    }
  }
}
