#pragma once

#include "mpask/AliasDeclaration.hxx"

#include <vector>

namespace mpask
{
  class Kober
  {
  public:
    Kober(const AliasDeclaration&);
    std::vector<unsigned char> operator()(const std::string&) const;
  private:
    const AliasDeclaration& alias;
    unsigned char calculateVisibilityBytes() const;
    std::vector<unsigned char> encodeNull() const;
    std::vector<unsigned char> encodeInteger(const std::string&) const;
    std::vector<unsigned char> encodeOctetString(const std::string&) const;
  };
}
