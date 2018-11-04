#pragma once

#include <string>
#include <ostream>

namespace mpask {

struct Definition
{
  friend std::ostream& operator<<(std::ostream& os, const Definition&);
  std::string identifier;
};

}
