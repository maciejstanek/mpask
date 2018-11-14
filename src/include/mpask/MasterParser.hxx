#pragma once

#include "mpask/MIBFile.hxx"

#include <string>
#include <memory>

namespace mpask {

class MasterParser
{
public:
  std::shared_ptr<MIBFile> operator()(const std::string& dir, const std::string& fileName) const;
};

}
