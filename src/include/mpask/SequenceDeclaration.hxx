#pragma once

#include "mpask/Restriction.hxx"
#include "mpask/DataType.hxx"

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <string>
#include <map> // Ordered

namespace mpask {

typedef std::map<std::string, DataType> map_string_DataType;

struct SequenceDeclaration
{
  std::string name;
  std::string tag; // {SEQUENCE, CHOICE}
  map_string_DataType sequence;
};

}

// TODO: Is it needed?
BOOST_FUSION_ADAPT_STRUCT(
  mpask::SequenceDeclaration,
  (std::string, name)
  (std::string, tag)
  (mpask::map_string_DataType, sequence)
)
