#include "mpask/MasterParser.hxx"

#include "mpask/Parser.hxx"
#include "mpask/Exception.hxx"

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
      string basePath {dir + "/"s + remainingFiles.front()};
      ifstream input {basePath + ".mib"s};
      if (!input.good()) {
        input.open(basePath + ".txt"s);
        if (!input.good()) {
          throw Exception {"neither "s + basePath + ".mib nor "s + basePath + ".txt found"s};
        }
      }
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
    return masterStructure;
  }
}
