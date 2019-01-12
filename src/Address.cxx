#include "mpask/Address.hxx"

#include <sstream>

using namespace std;

namespace mpask
{
  ostream&
  operator<<(ostream& output, const Address& address)
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

  bool
  operator==(const Address& lhs, const Address& rhs)
  {
    if (lhs.label != rhs.label) {
      return false;
    }
    if (lhs.value != rhs.value) {
      return false;
    }
    if (lhs.intermediateNodes.size() != rhs.intermediateNodes.size()) {
      return false;
    }
    for (auto i = lhs.intermediateNodes.begin(), j = rhs.intermediateNodes.begin();
        i != lhs.intermediateNodes.end(); i++, j++) {
      if (i->first != j->first) {
        return false;
      }
      if (i->second != j->second) {
        return false;
      }
    }
    return true;
  }

  bool
  operator!=(const Address& lhs, const Address& rhs)
  {
    return !(lhs == rhs);
  }
}
