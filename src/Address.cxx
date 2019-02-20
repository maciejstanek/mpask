#include "mpask/Address.hxx"
#include "mpask/Exception.hxx"

#include <sstream>

using namespace std;

namespace mpask
{
  Address::Address(const std::vector<int>& v)
  {
    if (v.size() <= 1u) {
      throw Exception {"at least two values are needed for a proper address"};
    }
    if (v[0] != 1) {
      throw Exception {"only iso(1) root note is supported"};
    }
    label = "iso";
    value = v.back();
    for (auto i = v.begin() + 1; i != v.end() - 1; ++i) {
      intermediateNodes.push_back({"", *i});
    }
  }

  Address::Address(const std::string& a, const vector_pair_string_int& b, int c)
    : label {a}, intermediateNodes {b}, value {c}
  {
  }

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
