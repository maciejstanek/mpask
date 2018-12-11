#pragma once

#include <string>

namespace mpask
{
  struct DataSequenceElement
  {
    virtual ~DataSequenceElement() = default;
    virtual const std::string& getType() const = 0;
    virtual void setType(const std::string&) = 0;
  };
}
