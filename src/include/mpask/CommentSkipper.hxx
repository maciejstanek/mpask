#pragma once

#include <istream>
#include <ostream>

namespace mpask {

class CommentSkipper
{
public:
  void skip(std::istream& input, std::ostream& output) const;
};

}
