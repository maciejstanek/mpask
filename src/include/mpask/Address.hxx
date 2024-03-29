#pragma once

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <string>
#include <vector>
#include <tuple>

namespace mpask {

typedef std::vector<std::pair<std::string, int>> vector_pair_string_int;

struct Address
{
  std::string label;
  vector_pair_string_int intermediateNodes;
  int value {};

  Address() = default;
  Address(const std::string&, const vector_pair_string_int&, int);
  Address(const std::vector<int>&);
  std::vector<int> toVector() const;
};

std::ostream& operator<<(std::ostream&, const Address&);
bool operator==(const Address&, const Address&);
bool operator!=(const Address&, const Address&);

}

// TODO: Is it needed?
BOOST_FUSION_ADAPT_STRUCT(
  mpask::Address,
  (std::string, label)
  /* (std::vector<std::pair<std::string, int>>, intermediateNodes) */
  (mpask::vector_pair_string_int, intermediateNodes)
  (int, value)
)
