#include "mpask/ValueKober.hxx"

#include "mpask/LengthKober.hxx"
#include "mpask/TagKober.hxx"

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <limits>

using namespace std;

namespace mpask
{
  ValueKober::ValueKober()
  {
  }
  
  vector<unsigned char>
  ValueKober::operator()(const shared_ptr<DataValue>& value) const
  {
    vector<unsigned char> code;
    if (value->context.dataType.name.isNull) {
      code = encodeNull(value);
    }
    else if (value->context.dataType.name.isInteger) {
      code = encodeInteger(value);
    }
    else if (value->context.dataType.name.isOctetString) {
      code = encodeOctetString(value);
    }
    else {
      throw runtime_error {"Encoding '"s + value->context.dataType.name.name + "' not implemented."s};
    }
    annotateCode(code, value);
    return code;
  }

  void
  ValueKober::annotateCode(vector<unsigned char>& code, const shared_ptr<DataValue>& value) const
  {
    if (value->context.isExplicit) {
      // Wrap it up (65 03 02 01 05).
      // 1. Force UNIVERSAL PRIMITIVE.
      code.at(0) &= 0x1f;
      // 2. Calculate and insert length.
      auto length = LengthKober()(code.size());
      code.insert(code.begin(), length.begin(), length.end());
      // 3. Prepend new identifier.
      auto identifier = TagKober()(value->context.typeIdentifier);
      identifier.at(0) |= calculateVisibilityBytes(value) | 0x20;
      identifier.insert(identifier.end(), code.begin(), code.end());
      code = identifier;
    }
    else if (value->context.isImplicit) {
      // Use provided tag (44 01 05).
      auto identifier = TagKober()(value->context.typeIdentifier);
      identifier.at(0) |= calculateVisibilityBytes(value);
      identifier.insert(identifier.end(), code.begin() + 1, code.end()); // discard old tag
      code = identifier;
    }
    // Do not do anything in case of a primitive (02 01 05).
  }

  //return static_cast<unsigned char>(value->context.typeIdentifier);

  unsigned char
  ValueKober::calculateConstructedBit(const shared_ptr<DataValue>& value) const
  {
    // EXPLICIT = CONSTRUCTED
    if (value->context.isExplicit) {
     return 0x20;
    }
    if (value->context.isImplicit) {
      return 0x00;
    }
    // NO KEYWORD MEANS IMPLICIT
    return 0x00;
  }

  unsigned char
  ValueKober::calculateVisibilityBytes(const shared_ptr<DataValue>& value) const
  {
    unsigned char bytes = 0x80; // CONTEXT-SPECIFIC (10) is a default
    if (value->context.visibility == "UNIVERSAL"s) {
      bytes = 0x00; // (00)
    }
    if (value->context.visibility == "APPLICATION"s) {
      bytes = 0x40; // (01)
    }
    if (value->context.visibility == "PRIVATE"s) {
      bytes = 0xC0; // (11)
    }
    return bytes;
  }

  vector<unsigned char>
  ValueKober::encodeNull(const shared_ptr<DataValue>& value) const
  {
    auto tag = static_cast<unsigned char>(0x05);
    vector<unsigned char> coded;
    coded.push_back(calculateVisibilityBytes(value) | tag);
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
  ValueKober::encodeInteger(const shared_ptr<DataValue>& value) const
  {
    auto tag = static_cast<unsigned char>(0x02);
    vector<unsigned char> coded;
    coded.push_back(calculateVisibilityBytes(value) | tag);
    long long inputConverted = stoul(value->getValue());
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
  ValueKober::encodeOctetString(const shared_ptr<DataValue>& value) const
  {
    auto tag = static_cast<unsigned char>(0x04);
    vector<unsigned char> coded;
    coded.push_back(calculateVisibilityBytes(value) | tag);
    // Only here we can get length > 127.
    // TODO It would be better to use length coder consistently but this is an uni project so who cares.
    auto length = LengthKober()(value->getValue().size());
    coded.insert(coded.end(), length.begin(), length.end());
    for (const auto& x : value->getValue()) {
      coded.push_back(static_cast<unsigned char>(x));
    }
    return coded;
  }
}
