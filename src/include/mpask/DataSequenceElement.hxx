#pragma once

#include <string>
#include "mpask/AliasDeclaration.hxx"

namespace mpask
{
  struct DataSequenceElement
  {
    virtual ~DataSequenceElement() = default;
    virtual const std::string& getType() const = 0;
    virtual void setType(const std::string&) = 0;
    virtual std::ostream& print(std::ostream&) const = 0;
  };

  std::ostream& operator<<(std::ostream& s, const DataSequenceElement& element);
}
