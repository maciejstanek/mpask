#include "mpask/TypeDeclaration.hxx"

#include <sstream>

using namespace std;

namespace mpask
{
  ostream& operator<<(ostream& output, const TypeDeclaration& type)
  {
    stringstream indices;
    bool first {true};
    for (const auto& index : type.indices) {
      if (!first) {
        indices << ", ";
      }
      indices << '"' << index << '"';
      first = false;
    }

    return output << "{"
			<< "\n  \"name\": \"" << type.name << "\","
			<< "\n  \"base_type\": \"" << type.baseType.name << "\","
			<< "\n  \"syntax\": " << type.syntax << ","
			<< "\n  \"access\": \"" << type.access << "\","
			<< "\n  \"status\": \"" << type.status << "\","
      // NOT SHOWING DESCRIPTION
      << "\n  \"indices\": [" << indices.str() << "],"
      << "\n  \"address\": " << type.address
			<< "\n}";
  }
}
