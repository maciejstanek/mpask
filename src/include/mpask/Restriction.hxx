#pragma once

#include <boost/fusion/adapted/struct.hpp>
#include <string>

namespace mpask {

struct Restriction
{
  // Restriction() = default;
  // Restriction(Restriction&) = default;
  // Restriction(const Restriction&) = default;
  // Restriction(const std::string&, int, int);
  // Restriction(const int, int);
  // Restriction(int);
  bool size {false};
  bool range {false};
  int left {};
  int right {};
};

// Restriction::Restriction(const std::string& newType, int newLeft, int newRight)
//   : type {newType},
//     left {newLeft},
//     right {newRight}
// {
// }
// 
// Restriction::Restriction(int newLeft, int newRight)
//   : left {newLeft},
//     right {newRight}
// {
// }
// 
// Restriction::Restriction(int argument)
//   : left {argument},
//     right {argument}
// {
// }

}

BOOST_FUSION_ADAPT_STRUCT(
  mpask::Restriction,
  (std::string, type)
  (int, left)
  (int, right)
)
