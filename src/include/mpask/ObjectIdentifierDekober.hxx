#pragma once

#include "mpask/Address.hxx"

#include <vector>

namespace mpask
{
  class ObjectIdentifierDekober
  {
  public:
    Address operator()(const std::vector<unsigned char>&) const;
    std::vector<int> getVector(const std::vector<unsigned char>&) const;
  private:
    unsigned long long decodeOneValue(std::vector<unsigned char>::const_iterator&) const;
    std::pair<unsigned long long, unsigned long long> decodeFirstValue(unsigned char code) const;
    unsigned long long parseLength(std::vector<unsigned char>::const_iterator& i, const std::vector<unsigned char>::const_iterator& end) const;
  };
}
