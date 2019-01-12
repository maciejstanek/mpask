#include "mpask/ObjectIdentifierDekober.hxx"

#include "mpask/Exception.hxx"

#include <iostream>

using namespace std;

namespace mpask
{
  Address
  ObjectIdentifierDekober::operator()(const vector<unsigned char>& code) const
  {
    if (code.size() < 1) {
      throw Exception("Cannot decode empty OID.");
    }
    auto i = code.begin();
    auto [v1, v2] = decodeFirstValue(*i++);
    vector<unsigned long long> v = {v1, v2};
    while (i != code.end()) {
      v.push_back(decodeOneValue(i));
    }
    Address a;
    a.value = v.back();
    v.pop_back();
    for (auto vi : v) {
      a.intermediateNodes.push_back({"", vi});
    }
    return a;
  }

  pair<unsigned long long, unsigned long long>
  ObjectIdentifierDekober::decodeFirstValue
    ( unsigned char code
    ) const
  {
    if (code & 0x80) {
      throw Exception("First two address elements has to comply to equation 40*a+b<127.");
    }
    return {code / 40, code % 40};
  }

  unsigned long long
  ObjectIdentifierDekober::decodeOneValue
    ( vector<unsigned char>::const_iterator& i
    ) const
  {
    unsigned long long val = 0;
    bool proceed = false;
    do {
      decltype(val) septet = *i & 0x7f;
      val = (val << 7) | septet;
      proceed = *i++ & 0x80;
    }
    while (proceed);
    return val;
  }
}
