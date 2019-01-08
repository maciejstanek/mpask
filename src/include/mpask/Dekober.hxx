#pragma once

#include "mpask/DataSequenceElement.hxx"

#include <vector>
#include <tuple>
#include <memory>

namespace mpask
{
  class Dekober
  {
  public:
    explicit Dekober(const std::vector<unsigned char>&);
    std::shared_ptr<DataSequenceElement> operator()() const;
  private:
    std::tuple<int, bool, unsigned long long> parseIdentifier(std::vector<unsigned char>::const_iterator&) const;
    int parseClass(std::vector<unsigned char>::const_iterator&) const;
    bool parseConstructed(std::vector<unsigned char>::const_iterator&) const;
    unsigned long long parseTag(std::vector<unsigned char>::const_iterator&) const;
    unsigned long long parseLength(std::vector<unsigned char>::const_iterator&) const;
    std::shared_ptr<DataSequenceElement> switchContext(std::vector<unsigned char>::const_iterator&) const;
    std::shared_ptr<DataSequenceElement> parseSequence(std::vector<unsigned char>::const_iterator&, int length) const;
    std::shared_ptr<DataSequenceElement> parseInteger(std::vector<unsigned char>::const_iterator&, int length) const;
    std::shared_ptr<DataSequenceElement> parseOctetString(std::vector<unsigned char>::const_iterator&, int length) const;
    std::shared_ptr<DataSequenceElement> parseUnknown(std::vector<unsigned char>::const_iterator&, int length) const;
    void throwOnOutOfBounds(std::vector<unsigned char>::const_iterator&) const;
    const std::vector<unsigned char>& code;
  };
}
