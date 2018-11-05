#pragma once

/* #include <boost/fusion/adapted/struct.hpp> */
#include <string>
#include <vector>
#include <tuple>

namespace mpask {

struct Address
{
  std::string label;
  std::vector<std::pair<std::string, int>> intermediateNodes;
  int value;
};

}

// TODO: Is it needed?
// BOOST_FUSION_ADAPT_STRUCT(
//   mpask::Address,
//   (std::string, label)
//   (std::vector<std::pair<std::string, int>>, intermediateNodes)
//   (int, value)
// )
