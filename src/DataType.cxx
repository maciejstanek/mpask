#include "mpask/DataType.hxx"

#include <sstream>

using namespace std;

namespace mpask
{
  ostream& operator<<(ostream& output, const DataType& type)
  {
    stringstream integers;
    integers << "\n    \"integer_values\": {";
    bool first {true};
    for (const auto& [tag, integer] : type.integerValues) {
      if (!first) {
        integers << ",";
      }
      integers << "\n      \"" << tag << "\": " << integer;
      first = false;
    }
    integers << "\n    },";

    return output << "{"
      << "\n    \"name\": \"" << type.name.name << "\","
      << "\n    \"variant\": \"" << (type.isSequence ? "SEQUENCE" : "CHOICE") << "\","
      << integers.str()
      << "\n    \"restriction\": " << type.restriction
      << "\n  }";
  }
}