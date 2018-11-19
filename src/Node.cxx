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

  void Node::printDotFile(ostream& output) const
  {
    vector<int> identifiers {};
    output << "digraph mib {" << endl;
    printDotFileNodes(identifiers, output); 
    output << endl;
    printDotFileConnections(identifiers, output); 
    output << "}" << endl;
  }

  string Node::generateDotFileNodeName(vector<int> identifiers) const
  {
    stringstream nodeName;
    nodeName << "n";
    for (auto singleIdentifier : identifiers) {
      nodeName << "_" << singleIdentifier;
    }
    return nodeName.str();
  }

  string Node::generateOID(vector<int> identifiers) const
  {
    stringstream longIdentifier;
    bool firstIdentifier {true};
    for (auto singleIdentifier : identifiers) {
      if (!firstIdentifier) {
        longIdentifier << ".";
      }
      firstIdentifier = false;
      longIdentifier << singleIdentifier;
    }
    return longIdentifier.str();
  }

  void Node::printDotFileNodes(vector<int> identifiers, ostream& output) const
  {
    output << "  " << generateDotFileNodeName(identifiers)
      << R"( [shape="record",margin="0.1",width="0",height="0",label="{<name>)"
      << getName() << "|<oid>" << generateOID(identifiers) << R"(}"];)"
      << endl;
    for (const auto& child : children) {
      auto newIdentifiers = identifiers;
      newIdentifiers.push_back(child.second->getIdentifier());
      child.second->printDotFileNodes(newIdentifiers, output);
    }
  }

  void Node::printDotFileConnections(vector<int> identifiers, ostream& output) const
  {
    auto nodeName = generateDotFileNodeName(identifiers);
    for (const auto& child : children) {
      auto newIdentifiers = identifiers;
      newIdentifiers.push_back(child.second->getIdentifier());
      auto subnodeName = child.second->generateDotFileNodeName(newIdentifiers);
      output << "  " << nodeName << ":oid -> " << subnodeName << ":name;" << endl;
      child.second->printDotFileConnections(newIdentifiers, output);
    }
  }

  void Node::printHierarchy(ostream& output) const
  {
    vector<int> identifiers {}; //{getIdentifier()};
    printHierarchy(identifiers, output); 
  }

  void Node::printHierarchy(vector<int> identifiers, ostream& output) const
  {
    int tab = (static_cast<int>(identifiers.size()) >= 0) ? 2 * static_cast<int>(identifiers.size()) : 0;
    auto padding = string(tab, ' ');
    output << padding << getName() << "(";
    if (getIdentifier() >= 0) {
      output << generateOID(identifiers);
    }
    output << ")" << endl;
    for (const auto& child : children) {
      auto newIdentifiers = identifiers;
      newIdentifiers.push_back(child.second->getIdentifier());
      child.second->printHierarchy(newIdentifiers, output);
    }
  }
}
