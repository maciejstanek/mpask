#pragma once

#include "mpask/TypeDeclaration.hxx"

#include <memory>
#include <vector>
#include <map>
#include <string>
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
private:
  TypeDeclaration source;
  std::string name;
  int identifier;
  std::map<int, std::shared_ptr<Node>> children;
};

}
