#pragma once

#include "mpask/MIBFile.hxx"
#include "mpask/DataSequenceElement.hxx"

#include <vector>
#include <memory>

namespace mpask
{
  class KoberWrapper
  {
  public:
    KoberWrapper(const std::shared_ptr<MIBFile>&);
    std::vector<unsigned char> encode(const std::shared_ptr<DataSequenceElement>&) const;
  private:
    std::shared_ptr<MIBFile> mib;
  };
}
