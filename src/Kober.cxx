#include "mpask/Kober.hxx"

#include <iostream>
#include <stdexcept>

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
    auto tag = static_cast<unsigned char>(0x05);
    vector<unsigned char> coded;
    coded.push_back(calculateVisibilityBytes() | tag);
    coded.push_back(0x00);
    return coded;
  }

  vector<unsigned char>
  Kober::encodeInteger(const string& input) const
  {
    // TODO: Use dynamic length as a backup option only if the range/size is not provided
    auto tag = static_cast<unsigned char>(0x02);
    vector<unsigned char> coded;
    coded.push_back(calculateVisibilityBytes() | tag);
    unsigned long inputConverted = stoul(input);
    auto inputConvertedCopy = inputConverted;
    unsigned char length = 0; // Can be assumed it will never be > 127; thus a basic length encoding will be used
    while (inputConvertedCopy) {
      inputConvertedCopy = (inputConvertedCopy >> 8);
      ++length;
    }
    coded.push_back(length);
    auto insertionPoint = coded.end();
    for (int i = 0; i < length; ++i) {
      auto maskByteShift = 8 * i;
      auto currentMask = 0xffUL << maskByteShift;
      unsigned char currentByte = static_cast<unsigned char>((inputConverted & currentMask) >> maskByteShift);
      insertionPoint = coded.insert(insertionPoint, currentByte);
    }
    return coded;
  }

  vector<unsigned char>
  Kober::encodeOctetString(const string& input) const
  {
    auto tag = static_cast<unsigned char>(0x04);
    vector<unsigned char> coded;
    coded.push_back(calculateVisibilityBytes() | tag);
    throw runtime_error {"Encoding octet strings not implemented."};
    return coded;
  }
}
