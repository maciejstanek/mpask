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

  void Node::addChild(const std::shared_ptr<Node>& node)
  {
    children.push_back(node);
  }

  shared_ptr<Node> Node::findNodeByName(const std::string& neddle) const
  {
    for (const auto& child : children) {
      if (child->getName() == neddle) {
        return child;
      }
      if (auto result = child->findNodeByName(neddle)) {
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
}
