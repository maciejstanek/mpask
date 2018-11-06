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
    for (const auto& type : input.types) {
      cerr << "Found '" << type.name << "'." << endl;
      auto newNode = make_shared<Node>(type.name, type.address.value);
      newNode->setSource(type);
      descramblingMap[type.name] = newNode;
    }
    for (const auto& [typeName, type] : descramblingMap) {
      cerr << "Processing '" << type->getName() << "'." << endl;
    }
    return make_shared<Node>("", 0);
  }
}
