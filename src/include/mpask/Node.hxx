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
  using Iterator = std::map<int, std::shared_ptr<Node>>::const_iterator;
  Node(const std::string& newName, int newIdentifier);
  void addChild(int identifier, const std::shared_ptr<Node>&);
  std::shared_ptr<Node> findNodeByName(const std::string&) const;
  std::string getName() const;
  int getIdentifier() const;
  Iterator begin() const;
  Iterator end() const;
  int size() const;
  void setSource(const TypeDeclaration&);
  const TypeDeclaration& getSource() const; // FIXME: Make a pointer or reference.
  void printHierarchy(std::vector<int> identifiers, std::ostream&, const std::string& tab) const;
  void printHierarchy(std::ostream&, const std::string& tab = "  ") const;
  std::string generateOID(std::vector<int> identifiers) const;
  void printDotFile(std::ostream& output) const;
  void printDotFileNodes(std::vector<int> identifiers, std::ostream& output) const;
  std::string generateDotFileNodeName(std::vector<int> identifiers) const;
  void printDotFileConnections(std::vector<int> identifiers, std::ostream& output) const;
  std::shared_ptr<Node> findNodeByOID(const std::vector<int>&) const;
  std::shared_ptr<Node> findNodeByOID(const std::string&) const;
  std::shared_ptr<Node> findNodeByOID(const std::vector<int>&, std::vector<int>::const_iterator) const;
  std::vector<int> findOidByName(const std::string&) const;
  Iterator find(int) const;
  std::string oidToString(const std::vector<int>&) const;
  std::string oidToString(std::vector<int>::const_iterator beginIter, std::vector<int>::const_iterator endIter) const;
  std::vector<int> stringToOid(const std::string&) const;
private:
  TypeDeclaration source;
  std::string name;
  int identifier;
  std::map<int, std::shared_ptr<Node>> children;
};

}
