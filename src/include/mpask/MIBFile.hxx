#pragma once

#include "mpask/SequenceDeclaration.hxx"
#include "mpask/TypeDeclaration.hxx"

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <string>
#include <vector>
#include <tuple>

namespace mpask {

typedef std::vector<SequenceDeclaration> vector_SequenceDeclaration;
typedef std::vector<TypeDeclaration> vector_TypeDeclaration;
typedef std::vector<std::pair<std::vector<std::string>, std::string>> vector_pair_vector_string_string;

struct MIBFile
{
  std::string name;
  vector_pair_vector_string_string imports;
  vector_SequenceDeclaration sequences;
  vector_TypeDeclaration types;
};

}

// TODO: Is it needed?
BOOST_FUSION_ADAPT_STRUCT(
  mpask::MIBFile,
  (std::string, name)
  (mpask::vector_pair_vector_string_string, imports)
  (mpask::vector_SequenceDeclaration, sequences)
  (mpask::vector_TypeDeclaration, types)
)
