#pragma once

#include <vector>

namespace mpask
{
  class TagKober
  {
  public:
    std::vector<unsigned char> operator()(unsigned long long tag) const;
  };
}
