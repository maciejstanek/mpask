#pragma once

#include "mpask/Address.hxx"

/* #include <boost/fusion/adapted/struct.hpp> */
#include <string>

namespace mpask {

struct ObjectIdentifier
{
  std::string identifier;
  Address address;
};

}

// TODO: Is it needed?
// BOOST_FUSION_ADAPT_STRUCT(
//   mpask::ObjectIdentifier,
//   (std::string, identifier)
//   (Address, address)
// )
