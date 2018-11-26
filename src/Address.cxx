#include "mpask/Address.hxx"

#include <sstream>

using namespace std;

namespace mpask
{
  ostream& operator<<(ostream& output, const Address& address)
  {
    output << "["
      << "\n    \"" << address.label << "\",";
    for (const auto& [label, index] : address.intermediateNodes) {
      output << "\n    [\"" << label << "\", " << index << "],";
    }
    return output
      << "\n    " << address.value
      << "\n  ]";
  }
}
