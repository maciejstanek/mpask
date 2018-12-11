#include "mpask/Kober.hxx"

#include "mpask/DataSequence.hxx"
#include "mpask/DataValue.hxx"
#include "mpask/ValueKober.hxx"

#include <iostream>

using namespace std;

namespace mpask
{
  Kober::Kober(const shared_ptr<MIBFile>& schema)
    : mib {schema}
  {
  }
  
  vector<unsigned char>
  Kober::encode(const shared_ptr<DataSequenceElement>& element) const
  {
    vector<unsigned char> code;
    if (auto sequence = dynamic_pointer_cast<DataSequence>(element)) {
      code.push_back(0xaa);
      for (const auto& subelement : *sequence)
      {
        auto subcode = encode(subelement);
        code.insert(code.end(), subcode.begin(), subcode.end());
      }
    }
    else if (auto value = dynamic_pointer_cast<DataValue>(element)) {
      auto subcode = ValueKober()(value);
      code.insert(code.end(), subcode.begin(), subcode.end());
    }
    return code;
  }
}
