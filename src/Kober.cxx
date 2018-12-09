#include "mpask/Kober.hxx"

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <limits>

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
    throw runtime_error {"Complex type encoding not implemented."};
    return {};
  }

  unsigned char
  Kober::calculateTag() const
  {
    return static_cast<unsigned char>(alias.typeIdentifier);
  }

  unsigned char
  Kober::calculateConstructedBit() const
  {
    // EXPLICIT = CONSTRUCTED
    if (alias.isExplicit) {
     return 0x20;
    }
    if (alias.isImplicit) {
      return 0x00;
    }
    // NO KEYWORD MEANS IMPLICIT
    return 0x00;
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

  int
  calculateRequiredNumberOfBits(long long number)
  {
    int length = 0;
    if (number < 0) {
      number = -number;
      ++length;
    }
    while (number) {
      number = (number >> 1);
      ++length;
    }
    return length;
  }

  vector<unsigned char>
  Kober::encodeInteger(const string& input) const
  {
    auto tag = static_cast<unsigned char>(0x02);
    vector<unsigned char> coded;
    coded.push_back(calculateVisibilityBytes() | tag);
    long long inputConverted = stoul(input);
    // FOR NOW I DO NOT CARE ABOUT RANGES
    unsigned char bitLength = calculateRequiredNumberOfBits(inputConverted);
    while (bitLength % 8) ++bitLength;
    unsigned char length = bitLength / 8; // Can be assumed it will never be > 127; thus a basic length encoding will be used
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
