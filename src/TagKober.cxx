#include "mpask/TagKober.hxx"

#include <iostream>

using namespace std;

namespace mpask
{
  vector<unsigned char>
  TagKober::operator()(unsigned long long tag) const
  {
    if (tag < 31) {
      return {static_cast<unsigned char>(tag)};
    }
    vector<unsigned char> coded = {0x1f};
    auto continueBit = 0x00;
    while (tag) {
      auto septet = 0x7f & tag;
      tag >>= 7;
      coded.insert((coded.begin() + 1), septet | continueBit);
      continueBit = 0x80;
    }
    return coded;
  }
}
