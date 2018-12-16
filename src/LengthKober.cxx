#include "mpask/LengthKober.hxx"

using namespace std;

namespace mpask
{
  vector<unsigned char>
  LengthKober::operator()(unsigned long long length) const
  {
    if (length < 128) {
      return {static_cast<unsigned char>(length)};
    }
    vector<unsigned char> coded;
    while (length) {
      auto octet = static_cast<unsigned char>(length); // will trim
      length >>= 8;
      coded.insert(coded.begin(), octet);
    }
    coded.insert(coded.begin(), static_cast<unsigned char>(0x80 | coded.size()));
    return coded;
  }
}
