#pragma once

#include <boost/fusion/adapted/struct.hpp>
#include <string>

namespace mpask {

struct Restriction
{
  bool size {false};
  bool range {false};
  long left {};
  long right {};
};

/* bool operator==(const Restriction& l, const Restriction& r) */
/* { */
/*   return l.size == r.size */
/*     && l.range == r.range */
/*     && l.left == r.left */
/*     && l.right == r.right; */
/* } */

}

// TODO: Is it needed?
// BOOST_FUSION_ADAPT_STRUCT(
//   mpask::Restriction,
//   (std::string, type)
//   (int, left)
//   (int, right)
// )
