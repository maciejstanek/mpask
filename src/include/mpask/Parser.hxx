#pragma once

#include "mpask/MIBFile.hxx"

#include <istream>
#include <memory>

namespace mpask {

class Parser
{
public:
  std::shared_ptr<MIBFile> operator()(std::istream&) const;
};

}
