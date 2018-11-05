#pragma once

#include "mpask/Restriction.hxx"
#include "mpask/DataTypeName.hxx"

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <string>
#include <map> // Ordered

namespace mpask {

typedef std::map<std::string, DataTypeName> map_string_DataTypeName;

struct SequenceDeclaration
{
  std::string name;
  map_string_DataTypeName sequence;
};

}

// TODO: Is it needed?
BOOST_FUSION_ADAPT_STRUCT(
  mpask::SequenceDeclaration,
  (std::string, name)
  (mpask::map_string_DataTypeName, sequence)
)
