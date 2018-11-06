#include "mpask/Node.hxx"

#include "mpask/Exception.hxx"

#include <iostream>
#include <sstream>

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

  void Node::printHierarchy(ostream& output) const
  {
    vector<int> identifiers {getIdentifier()};
    printHierarchy(identifiers, output); 
  }

  void Node::printHierarchy(vector<int> identifiers, ostream& output) const
  {
    int tab = (static_cast<int>(identifiers.size()) - 1 >= 0) ? 2 * (static_cast<int>(identifiers.size()) - 1) : 0;
    auto padding = string(tab, ' ');
    stringstream longIdentifier;
    bool firstIdentifier {true};
    for (auto singleIdentifier : identifiers) {
      if (!firstIdentifier) {
        longIdentifier << ".";
      }
      firstIdentifier = false;
      longIdentifier << singleIdentifier;
    }
    output << padding << getName() << "("  << longIdentifier.str() << ")" << endl;
    for (const auto& child : children) {
      auto newIdentifiers = identifiers;
      newIdentifiers.push_back(child.second->getIdentifier());
      child.second->printHierarchy(newIdentifiers, output);
    }
  }
}
