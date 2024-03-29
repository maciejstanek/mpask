#include "mpask/ObjectIdentifierKober.hxx"

#include "mpask/Exception.hxx"
#include "mpask/LengthKober.hxx"

#include <iostream>

using namespace std;

namespace mpask
{
  vector<unsigned char>
  ObjectIdentifierKober::operator()(const Address& address) const
  {
    vector<unsigned long long> numbers;
    for(const auto& node : address.intermediateNodes) {
      numbers.push_back(node.second);
    }
    numbers.push_back(address.value);
    if (numbers.size() > 1) {
      numbers[1] = 40 * numbers[0] + numbers[1];
      numbers.erase(numbers.begin());
    }
    else {
      // It is guaranteed that there is at least one number.
      numbers[0] *= 40;
    }
    if (numbers[0] > 127) {
      throw Exception("Cannot encode the first two values (40*a+b > 127).");
    }
    vector<unsigned char> code;
    for(auto number : numbers) {
      auto subcode = encodeOneValue(number);
      code.insert(code.end(), subcode.begin(), subcode.end());
    }
    int len = code.size();
    auto lencode = LengthKober()(len);
    code.insert(code.begin(), lencode.begin(), lencode.end());
    code.insert(code.begin(), 0x06);
    return code;
  }

  vector<unsigned char>
  ObjectIdentifierKober::encodeOneValue(unsigned long long number) const
  {
    vector<unsigned char> code;
    while (number) {
      code.insert(code.begin(), static_cast<unsigned char>(number | 0x80));
      number >>= 7;
    }
    if (code.size() == 0) {
      code.push_back(0);
    }
    code.back() &= 0x7f;
    return code;
  }

  vector<unsigned char>
  ObjectIdentifierKober::operator()(const std::vector<int>& oid) const
  {
    auto numbers = oid;
    if (numbers.size() > 1) {
      numbers[1] = 40 * numbers[0] + numbers[1];
      numbers.erase(numbers.begin());
    }
    else {
      // It is guaranteed that there is at least one number.
      numbers[0] *= 40;
    }
    if (numbers[0] > 127) {
      throw Exception("Cannot encode the first two values (40*a+b > 127).");
    }
    vector<unsigned char> code;
    for(auto number : numbers) {
      auto subcode = encodeOneValue(number);
      code.insert(code.end(), subcode.begin(), subcode.end());
    }
    int len = code.size();
    auto lencode = LengthKober()(len);
    code.insert(code.begin(), lencode.begin(), lencode.end());
    code.insert(code.begin(), 0x06);
    return code;
  }
}
