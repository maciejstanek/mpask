#include "mpask/MasterParser.hxx"

#include "mpask/Parser.hxx"

#include <iostream>
#include <string>
#include <queue>
#include <fstream>

using namespace std;

namespace mpask
{
  shared_ptr<MIBFile>
  MasterParser::operator()
    ( const string& dir
    , const string& fileName
    ) const
  {
    shared_ptr<MIBFile> masterStructure;
    queue<string> remainingFiles {{fileName}};
    while (!remainingFiles.empty()) {
      ifstream input {dir + "/"s + remainingFiles.front() + ".mib"s};
      auto object = Parser{}(input);
      for (const auto& import : object->imports) {
        if (import.second == "RFC-1212") continue; // NOTE: Hardcoding ignoring RFC-1212.mib
        remainingFiles.push(import.second);
      }
      if (!masterStructure) {
        masterStructure = move(object);
      }
      else {
        masterStructure->meld(*object);
      }
      remainingFiles.pop();
    }
    return move(masterStructure);
  }
}
