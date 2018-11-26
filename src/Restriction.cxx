#include "mpask/Restriction.hxx"

#include <sstream>

using namespace std;

namespace mpask
{
  ostream& operator<<(ostream& output, const Restriction& restriction)
  {
    return output << "{"
      << "\n      \"size\": " << (restriction.size ? "true" : "false") << ","
      << "\n      \"range\": " << (restriction.range ? "true" : "false") << ","
      << "\n      \"left\": " << restriction.left << ","
      << "\n      \"right\": " << restriction.right
      << "\n    }";
  }
}
