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
      auto currentParentIter = descramblingMap.find(type->getSource().address.label);
      if (currentParentIter == descramblingMap.end()) {
        cerr << "skipping free floating node '" << typeName << "'" << endl;
        continue;
      }
      shared_ptr<Node> currentParent = currentParentIter->second;
      // Expand complex address {{{
      for (const auto& [intermediateLabel, intermediateIdentifier] : type->getSource().address.intermediateNodes) {
        cerr << "intermediate " << intermediateLabel << "(" << intermediateIdentifier << ")" << endl;
        auto intermediateNodeIter = descramblingMap.find(intermediateLabel);
        if (descramblingMap.find(intermediateLabel) == descramblingMap.end()) {
          cerr << "adding" << endl;
          // Add node if not defined
          descramblingMap[intermediateLabel] = make_shared<Node>(intermediateLabel, intermediateIdentifier);
          intermediateNodeIter = descramblingMap.find(intermediateLabel);
        }
        auto intermediateNode = intermediateNodeIter->second;
        currentParent->addChild(intermediateIdentifier, intermediateNode);
        currentParent = intermediateNode;
      }
      // }}}
      currentParent->addChild(type->getSource().address.value, type);
    }
    return root;
  }
}
