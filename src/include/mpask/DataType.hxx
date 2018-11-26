#pragma once

#include "mpask/Restriction.hxx"
#include "mpask/DataTypeName.hxx"

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <ostream>

namespace mpask {

typedef std::map<std::string, int> map_string_int;

struct DataType
{
  DataTypeName name;
  bool isSequence {false}; // SEQUENCE OF 'name'
  map_string_int integerValues;
  Restriction restriction;
};

std::ostream& operator<<(std::ostream&, const DataType&);

}

// TODO: Is it needed?
BOOST_FUSION_ADAPT_STRUCT(
  mpask::DataType,
  (mpask::DataTypeName, name)
  (bool, isSequence)
  (mpask::map_string_int, integerValues)
  (mpask::Restriction, restriction)
)
