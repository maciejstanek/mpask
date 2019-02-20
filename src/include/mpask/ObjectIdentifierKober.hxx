#pragma once

#include "mpask/Address.hxx"

#include <vector>

namespace mpask
{
  class ObjectIdentifierKober
  {
  public:
    std::vector<unsigned char> operator()(const Address&) const;
    std::vector<unsigned char> operator()(const std::vector<int>&) const;
  private:
    std::vector<unsigned char> encodeOneValue(unsigned long long) const;
  };
}
