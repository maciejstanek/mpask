#include "mpask/Dekober.hxx"

#include "mpask/Exception.hxx"

#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

namespace mpask
{
  Dekober::Dekober(const std::vector<unsigned char>& parsedCode)
    : code {parsedCode}
  {
  }

  int
  Dekober::operator()() const
  {
    auto i = code.begin();
    if (i == code.end()) {
      // TODO: Return empty pointer or sth
      return 0;
    }
    switchContext(i);
    if (i != code.end()) {
      stringstream s;
      s << "Byte vector ended unexpectedly at element #"
        << distance(code.begin(), i) << " (0x"s << setw(2) << setfill('0')
        << hex << static_cast<int>(*i) << ")."s;
      throw Exception {s.str()};
    }
    return 0; // TODO: Temporallily. Will return object tree.
  }

  void
  Dekober::switchContext(std::vector<unsigned char>::const_iterator& i) const
  {
    auto [objectClass, constructed, tag] = parseIdentifier(i);
    cerr << "CLASS = " << objectClass << "\n";
    cerr << "CONSTRUCTED = " << constructed << "\n";
    cerr << "TAG = " << dec << tag << "\n";
    auto length = parseLength(i);
    cerr << "LENGTH = " << length << "\n";
    switch (tag) {
      case 0x02:
        cerr << "TYPE = INTEGER\n";
        parseInteger(i, length);
        break;
      case 0x04:
        cerr << "TYPE = OCTET STRING\n";
        parseOctetString(i, length);
        break;
      case 0x10:
        cerr << "TYPE = SEQUENCE\n";
        parseSequence(i, length);
        break;
      default:
        cerr << "TYPE = UNKNOWN " << tag << "\n";
        parseUnknown(i, length);
    }
  }

  void
  Dekober::parseSequence(std::vector<unsigned char>::const_iterator& i, int length) const
  {
    auto start = i;
    while (i < start + length) {
      switchContext(i);
    }
    if (start + length != i) {
      throw Exception {"Specified length '"s + to_string(length) + "' is not matching the sequence end."s};
    }
  }

  void
  Dekober::parseInteger(std::vector<unsigned char>::const_iterator& i, int length) const
  {
    unsigned long long number = 0;
    while (length--) {
      throwOnOutOfBounds(i);
      number = (number << 8) | *i++;
    }
    cerr << "VALUE = " << dec << number << "\n";
  }

  void
  Dekober::parseOctetString(std::vector<unsigned char>::const_iterator& i, int length) const
  {
    stringstream s;
    while (length--) {
      throwOnOutOfBounds(i);
      s << *i++;
    }
    cerr << "VALUE = " << s.str() << "\n";
  }

  void
  Dekober::parseUnknown(std::vector<unsigned char>::const_iterator& i, int length) const
  {
    vector<unsigned char> v;
    while (length--) {
      throwOnOutOfBounds(i);
      v.push_back(*i++);
    }
    cerr << "VALUE =";
    for(auto vi : v) {
      cerr << " 0x" << hex << setw(2) << setfill('0') << static_cast<int>(vi);
    }
    cerr << "\n";
  }

  unsigned long long
  Dekober::parseLength(std::vector<unsigned char>::const_iterator& i) const
  {
    throwOnOutOfBounds(i);
    unsigned long long length = *i++;
    if (length > 0x7f) {
      decltype(length) lengthOfLength {length & 0x7f};
      length = 0;
      while(lengthOfLength--) {
        throwOnOutOfBounds(i);
        // TODO: Check 64 bit overflow.
        length = (length << 8) | *i++;
      }
    }
    return length;
  }

  tuple<int, bool, unsigned long long>
  Dekober::parseIdentifier(std::vector<unsigned char>::const_iterator& i) const
  {
    return {parseClass(i), parseConstructed(i), parseTag(i)};
  }

  int
  Dekober::parseClass(std::vector<unsigned char>::const_iterator& i) const
  {
    throwOnOutOfBounds(i);
    return static_cast<unsigned char>(*i >> 6);
  }

  bool
  Dekober::parseConstructed(std::vector<unsigned char>::const_iterator& i) const
  {
    throwOnOutOfBounds(i);
    return *i & 0x20;
  }

  unsigned long long
  Dekober::parseTag(std::vector<unsigned char>::const_iterator& i) const
  {
    throwOnOutOfBounds(i);
    unsigned long long tag = *i++ & 0x1f;
    if (tag == 0x1f) {
      tag = 0;
      // TODO: Tag number is constrained by 64 bits. If its bigger, no exception is thrown.
      unsigned char septet = 0x80;
      while (septet & 0x80) {
        tag <<= 7;
        throwOnOutOfBounds(i);
        septet = *i++;
        tag |= 0x7f & septet;
      }
    }
    return tag;
  }

  void 
  Dekober::throwOnOutOfBounds(std::vector<unsigned char>::const_iterator& i) const
  {
    if (i == code.end()) {
      throw Exception {"Byte vector ended unexpectedly at the last element."};
    }
  }
}
