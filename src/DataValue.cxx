#include "mpask/DataValue.hxx"

using namespace std;

namespace mpask
{
  const string&
  DataValue::getType() const
  {
    return type;
  }

  void
  DataValue::setType(const string& x)
  {
    type = x;
  }

  const string&
  DataValue::getValue() const
  {
    return value;
  }

  void
  DataValue::setValue(const string& x)
  {
    value = x;
  }
}
