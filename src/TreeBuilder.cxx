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
  TreeBuilder::operator()(const shared_ptr<MIBFile>& input) const
  {
    map<string, shared_ptr<Node>> descramblingMap;

    // NOTE: Hardcoded root node
    auto root = make_shared<Node>("root", -1);
    TypeDeclaration rootType;
    rootType.address.label = ""; // No parent
    rootType.address.value = -1; // No parent
    root->setSource(move(rootType));
    descramblingMap["root"] = root;

    // NOTE: Hardcoded iso node
    auto iso = make_shared<Node>("iso", 1);
    TypeDeclaration isoType;
    isoType.address.label = "root";
    isoType.address.value = 1;
    iso->setSource(move(isoType));
    descramblingMap["iso"] = iso;

    for (const auto& type : input->types) {
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
    // NOTE: Hardcoded
    /* for (const auto& [typeName, type] : descramblingMap) { */
    /*   cerr << "Check " << typeName << endl; */
    /*   if (type->getSource().address.label == "iso"s) { */
    /*     cerr << "Match!" << endl; */
    /*     iso->addChild(type->getSource().address.value, type); */
    /*   } */
    /* } */
    return root;
  }
}
