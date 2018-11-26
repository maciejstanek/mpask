#include "mpask/Node.hxx"

#include "mpask/Exception.hxx"

#include <iostream>
#include <sstream>

using namespace std;

namespace mpask
{
  Node::Node(const string& newName, int newIdentifier)
    : name {newName},
      identifier {newIdentifier}
  {
  }

  void Node::addChild(int nodeIdentifier, const shared_ptr<Node>& node)
  {
    children[nodeIdentifier] = node;
  }

  shared_ptr<Node> Node::findNodeByName(const string& neddle) const
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

  string Node::getName() const
  {
    return name;
  }

  int Node::getIdentifier() const
  {
    return identifier;
  }

  Node::Iterator Node::begin() const
  {
    return children.begin();
  }

  Node::Iterator Node::end() const
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

  void Node::printHierarchy(ostream& output, const string& tab) const
  {
    vector<int> identifiers {};
    printHierarchy(identifiers, output, tab); 
  }

  void Node::printHierarchy(vector<int> identifiers, ostream& output, const string& tab) const
  {
    int tabCount = static_cast<int>(identifiers.size());
    string padding;
    while (--tabCount >= 0) {
      padding += tab;
    }
    output << padding << getName() << "(";
    if (getIdentifier() >= 0) {
      output << generateOID(identifiers);
    }
    output << ")" << endl;
    for (const auto& child : children) {
      auto newIdentifiers = identifiers;
      newIdentifiers.push_back(child.second->getIdentifier());
      child.second->printHierarchy(newIdentifiers, output, tab);
    }
  }

  shared_ptr<Node>
  Node::findNodeByOID(const string& oid) const
  {
    auto oidVector = stringToOid(oid);
    return findNodeByOID(oidVector, oidVector.begin());
  }

  shared_ptr<Node>
  Node::findNodeByOID(const vector<int>& oid) const
  {
    return findNodeByOID(oid, oid.begin());
  }

  shared_ptr<Node>
  Node::findNodeByOID(const vector<int>& oid, vector<int>::const_iterator current) const
  {
    auto next = find(*current);
    if (next == end()) {
      throw Exception {"Could not find OID '"s + oidToString(oid) + "'. "s
        + "Failed on searching for '"s + oidToString(oid.begin(), current + 1) + "'."};
    }
    if (++current == oid.end()) {
      return next->second;
    }
    return next->second->findNodeByOID(oid, current);
  }

  Node::Iterator
  Node::find(int id) const
  {
    return children.find(id);
  }

  string
  Node::oidToString(const vector<int>& oid) const
  {
    return oidToString(oid.begin(), oid.end());
  }

  string
  Node::oidToString(vector<int>::const_iterator beginIter, vector<int>::const_iterator endIter) const
  {
    bool first {true};
    string message;
    for (auto iter = beginIter; iter != endIter; ++iter) {
      if (!first) {
        message += ".";
      }
      message += to_string(*iter);
      if (first) {
        first = false;
      }
    }
    return message;
  }

  vector<int>
  Node::stringToOid(const string& rawOid) const
  {
    istringstream oidStream(rawOid);
    string id;
    vector<int> oid;
    while(getline(oidStream, id, '.')) {
      oid.push_back(stoi(id));
    }
    return oid;
  }

  vector<int>
  Node::findOidByName(const string& searchedName) const
  {
    // FIXME: Should be BFS, is DFS.
    for (auto iter = begin(); iter != end(); ++iter) {
      auto node = iter->second;
      vector<int> oid {node->getIdentifier()};
      if (node->getName() == searchedName) {
        return oid;
      }
      auto next = node->findOidByName(searchedName);
      if (next.size() > 0) {
        oid.insert(oid.end(), next.begin(), next.end());
        return oid;
      }
    }
    return {};
  }

  void
  Node::printNode(std::ostream& output) const
  {
    output << source;
  }
}
