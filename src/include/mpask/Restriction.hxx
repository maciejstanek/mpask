#pragma once

#include <boost/fusion/adapted/struct.hpp>
#include <string>

namespace mpask {

struct Restriction
{
  bool size {false};
  bool range {false};
  int left {};
  int right {};
};

}

// TODO: Is it needed?
// BOOST_FUSION_ADAPT_STRUCT(
//   mpask::Restriction,
//   (std::string, type)
//   (int, left)
//   (int, right)
// )
