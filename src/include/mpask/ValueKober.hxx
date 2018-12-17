#pragma once

#include "mpask/DataValue.hxx"

#include <vector>
#include <memory>

namespace mpask
{
  class ValueKober
  {
  public:
    ValueKober();
    std::vector<unsigned char> operator()(const std::shared_ptr<DataValue>&) const;
  private:
    void annotateCode(std::vector<unsigned char>&, const std::shared_ptr<DataValue>&) const;
    unsigned char calculateIdentifier(const std::shared_ptr<DataValue>&) const;
    unsigned char calculateVisibilityBytes(const std::shared_ptr<DataValue>&) const;
    unsigned char calculateTag(const std::shared_ptr<DataValue>&) const;
    unsigned char calculateConstructedBit(const std::shared_ptr<DataValue>&) const;
    std::vector<unsigned char> encodeNull(const std::shared_ptr<DataValue>&) const;
    std::vector<unsigned char> encodeInteger(const std::shared_ptr<DataValue>&) const;
    std::vector<unsigned char> encodeOctetString(const std::shared_ptr<DataValue>&) const;
  };

  int calculateRequiredNumberOfBits(long long);
}
