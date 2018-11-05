#pragma once

#include "mpask/Restriction.hxx"
#include "mpask/DataTypeName.hxx"
#include "mpask/DataType.hxx"
#include "mpask/Address.hxx"

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <string>
#include <vector>
#include <tuple>

namespace mpask {

typedef std::vector<std::string> vector_string;

struct TypeDeclaration
{
  std::string name;
  DataTypeName baseType;
  DataType syntax;
  std::string access;
  std::string status;
  std::string description;
  vector_string indices;
  Address address;
};

}

// TODO: Is it needed?
BOOST_FUSION_ADAPT_STRUCT(
  mpask::TypeDeclaration,
  (std::string, name)
  (mpask::DataTypeName, baseType)
  (mpask::DataType, syntax)
  (std::string, access)
  (std::string, status)
  (std::string, description)
  (mpask::vector_string, indices)
  (mpask::Address, address)
)
