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
      code.push_back(0x30);
      vector<unsigned char> sequenceCode;
      for (const auto& subelement : *sequence)
      {
        auto subcode = encode(subelement);
        sequenceCode.insert(sequenceCode.end(), subcode.begin(), subcode.end());
      }
      code.push_back(static_cast<unsigned char>(sequenceCode.size())); // TODO: implement length > 127
      code.insert(code.end(), sequenceCode.begin(), sequenceCode.end());
    }
    else if (auto value = dynamic_pointer_cast<DataValue>(element)) {
      auto subcode = ValueKober()(value);
      code.insert(code.end(), subcode.begin(), subcode.end());
    }
    return code;
  }
}
