#include "mpask/Definition.hxx"

using namespace std;

namespace mpask
{
  ostream& operator<<(ostream& output, const Definition& definition)
  {
    return output << "{"
      << endl << "  \"identifier\": \"" << definition.identifier << "\""
      << endl << "}";
  }
}
