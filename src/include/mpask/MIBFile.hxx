#pragma once

#include "mpask/SequenceDeclaration.hxx"
#include "mpask/TypeDeclaration.hxx"
#include "mpask/AliasDeclaration.hxx"

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <string>
#include <vector>
#include <tuple>

namespace mpask {

typedef std::vector<SequenceDeclaration> vector_SequenceDeclaration;
typedef std::vector<TypeDeclaration> vector_TypeDeclaration;
typedef std::vector<AliasDeclaration> vector_AliasDeclaration;
typedef std::vector<std::pair<std::vector<std::string>, std::string>> vector_pair_vector_string_string;
typedef std::vector<std::string> vector_string;

struct MIBFile
{
  std::string name;
  vector_pair_vector_string_string imports;
  vector_string exports;
  vector_SequenceDeclaration sequences;
  vector_TypeDeclaration types;
  vector_AliasDeclaration aliases;
  void meld(const MIBFile&);
};

}

// TODO: Is it needed?
BOOST_FUSION_ADAPT_STRUCT(
  mpask::MIBFile,
  (std::string, name)
  (mpask::vector_pair_vector_string_string, imports)
  (mpask::vector_string, exports)
  (mpask::vector_SequenceDeclaration, sequences)
  (mpask::vector_TypeDeclaration, types)
  (mpask::vector_AliasDeclaration, aliases)
)
