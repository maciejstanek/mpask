#include "mpask/Kober.hxx"

#include <iostream>

using namespace std;

namespace mpask
{
  Kober::Kober(const AliasDeclaration& codedAlias)
    : alias {codedAlias}
  {
  }
  
  vector<unsigned char>
  Kober::operator()(const string& input) const
  {
    cerr << "CALLED" << endl;
    return {};
  }
}
