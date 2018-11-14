#include "mpask/MasterParser.hxx"

#include "mpask/Parser.hxx"

#include <iostream>
#include <string>
#include <sstream>
#include <streambuf>

using namespace std;

namespace mpask
{
  shared_ptr<MIBFile>
  MasterParser::operator()
    ( const string& dir
    , const string& fileName
    ) const
  {
    return make_shared<MIBFile>();
  }
}
