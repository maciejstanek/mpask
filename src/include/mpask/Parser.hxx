#pragma once

#include "mpask/MIBFile.hxx"

#include <istream>

namespace mpask {

class Parser
{
public:
  MIBFile operator()(std::istream&) const;
};

}
