#pragma once

#include "mpask/TypeDeclaration.hxx"

#include <memory>
#include <vector>
#include <string>
// #include <variant>

namespace mpask {

class Node
{
public:
  using Iterator = std::vector<std::shared_ptr<Node>>::iterator;
  Node(const std::string& newName, int newIdentifier);
  void addChild(const std::shared_ptr<Node>&);
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
  std::vector<std::shared_ptr<Node>> children;
};

}
