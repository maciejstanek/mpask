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
  };
}
