#pragma once

#include "mpask/TypeDeclaration.hxx"

#include <memory>
#include <vector>
#include <map>
#include <string>
#include <ostream>
// #include <variant>

namespace mpask {

class Node
{
public:
  using Iterator = std::map<int, std::shared_ptr<Node>>::iterator;
  Node(const std::string& newName, int newIdentifier);
  void addChild(int identifier, const std::shared_ptr<Node>&);
  std::shared_ptr<Node> findNodeByName(const std::string&) const;
  std::string getName() const;
  int getIdentifier() const;
  Iterator begin();
  Iterator end();
  int size() const;
  void setSource(const TypeDeclaration&);
  const TypeDeclaration& getSource() const; // FIXME: Make a pointer or reference.
  void printHierarchy(std::vector<int> identifiers, std::ostream&) const;
  void printHierarchy(std::ostream&) const;
  std::string generateOID(std::vector<int> identifiers) const;
  void printDotFile(std::ostream& output) const;
  void printDotFileNodes(std::vector<int> identifiers, std::ostream& output) const;
  std::string generateDotFileNodeName(std::vector<int> identifiers) const;
  void printDotFileConnections(std::vector<int> identifiers, std::ostream& output) const;
private:
  TypeDeclaration source;
  std::string name;
  int identifier;
  std::map<int, std::shared_ptr<Node>> children;
};

}
