#pragma once

#include <vector>

namespace mpask
{
  class LengthKober
  {
  public:
    std::vector<unsigned char> operator()(unsigned long long length) const;
  };
}
