#pragma once

#include "mpask/MIBFile.hxx"
#include "mpask/Node.hxx"

#include <memory>

namespace mpask {

class TreeBuilder
{
public:
  std::shared_ptr<Node> operator()(const MIBFile&) const;
};

}
