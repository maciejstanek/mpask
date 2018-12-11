#include "mpask/KoberWrapper.hxx"

#include "mpask/DataSequence.hxx"
#include "mpask/DataValue.hxx"
#include "mpask/Kober.hxx"

#include <iostream>

using namespace std;

namespace mpask
{
  KoberWrapper::KoberWrapper(const shared_ptr<MIBFile>& schema)
    : mib {schema}
  {
  }
  
  vector<unsigned char>
  KoberWrapper::encode(const shared_ptr<DataSequenceElement>& element) const
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
      auto subcode = Kober{value->getContextAlias()}(value->getValue());
      code.insert(code.end(), subcode.begin(), subcode.end());
    }
    return code;
  }
}
