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
    if (alias.dataType.name.isNull) {
      return encodeNull();
    }
    else if (alias.dataType.name.isInteger) {
      return encodeInteger(input);
    }
    else if (alias.dataType.name.isOctetString) {
      return encodeOctetString(input);
    }
    cerr << "TODO: encode complex types" << endl;
    return {};
  }

  unsigned char
  Kober::calculateVisibilityBytes() const
  {
    unsigned char bytes = 0x80; // CONTEXT-SPECIFIC (10) is a default
    if (alias.visibility == "UNIVERSAL"s) {
      bytes = 0x00; // (00)
    }
    if (alias.visibility == "APPLICATION"s) {
      bytes = 0x40; // (01)
    }
    if (alias.visibility == "PRIVATE"s) {
      bytes = 0xC0; // (11)
    }
    return bytes;
  }

  vector<unsigned char>
  Kober::encodeNull() const
  {
    return {0x05, 0x00};
  }

  vector<unsigned char>
  Kober::encodeInteger(const string& input) const
  {
    cerr << "TODO: encode integer" << endl;
    return {};
  }

  vector<unsigned char>
  Kober::encodeOctetString(const string& input) const
  {
    cerr << "TODO: encode octet string" << endl;
    return {};
  }
}
