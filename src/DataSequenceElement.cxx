#include "mpask/DataSequenceElement.hxx"

using namespace std;

namespace mpask
{
  ostream& operator<<(ostream& s, const DataSequenceElement& element)
  {
    return element.print(s);
  }
}
