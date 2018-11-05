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

struct AliasDeclaration
{
  std::string name;
  bool isExplicit {false};
  bool isImplicit {false};
  std::string visibility;
  int typeIdentifier {-1};
  DataType dataType;
};

}

// TODO: Is it needed?
BOOST_FUSION_ADAPT_STRUCT(
  mpask::AliasDeclaration,
  (std::string, name)
  (bool, isExplicit)
  (bool, isImplicit)
  (std::string, visibility)
  (int, typeIdentifier)
  (mpask::DataType, dataType)
)
