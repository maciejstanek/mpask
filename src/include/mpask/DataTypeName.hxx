#pragma once

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <string>

namespace mpask {

struct DataTypeName
{
  std::string name;
  bool isInteger {false};
  bool isOctetString {false};
  bool isObjectIdentifier {false};
  bool isNull {false};
};

}

// TODO: Is it needed?
BOOST_FUSION_ADAPT_STRUCT(
  mpask::DataTypeName,
  (std::string, name)
  (bool, isSequence)
  (bool, isOctetString)
  (bool, isObjectIdentifier)
  (bool, isNull)
)
