#include "mpask/TreeBuilder.hxx"

#include "mpask/Exception.hxx"
#include "mpask/TypeDeclaration.hxx"

#include <iostream>
#include <memory>
#include <map>

using namespace std;

namespace mpask
{
  shared_ptr<Node>
  TreeBuilder::operator()(const MIBFile& input) const
  {
    map<string, shared_ptr<Node>> descramblingMap;

    auto root = make_shared<Node>("mib-2", 0); // NOTE: HARDCODED
    descramblingMap["mib-2"] = root;

    for (const auto& type : input.types) {
      auto newNode = make_shared<Node>(type.name, type.address.value);
      newNode->setSource(type);
      descramblingMap[type.name] = newNode;
    }
    for (const auto& [typeName, type] : descramblingMap) {
      if (type->getSource().address.intermediateNodes.size() != 0) {
        throw Exception {"Not yet capable of building a tree from an extended address ("s + typeName + ")."s};
      }
      auto node = descramblingMap.find(type->getSource().address.label);
      if (node != descramblingMap.end()) {
        node->second->addChild(type->getSource().address.value, type);
      }
    }
    return root;
  }
}
