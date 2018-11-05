#pragma once

#include "mpask/Restriction.hxx"

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <string>
#include <vector>
#include <tuple>

namespace mpask {

typedef std::map<std::string, int> map_string_int;

struct DataType
{
  std::string name;
  bool isSequence {false}; // SEQUENCE OF 'name'
  map_string_int integerValues;
  Restriction restriction;
};

}

// TODO: Is it needed?
BOOST_FUSION_ADAPT_STRUCT(
  mpask::DataType,
  (std::string, name)
  (bool, isSequence)
  (mpask::map_string_int, integerValues)
  (mpask::Restriction, restriction)
)
