#pragma once

#include <vector>
#include <tuple>

namespace mpask
{
  class Dekober
  {
  public:
    explicit Dekober(const std::vector<unsigned char>&);
    int operator()() const;
  private:
    std::tuple<int, bool, unsigned long long> parseIdentifier(std::vector<unsigned char>::const_iterator&) const;
    int parseClass(std::vector<unsigned char>::const_iterator&) const;
    bool parseConstructed(std::vector<unsigned char>::const_iterator&) const;
    unsigned long long parseTag(std::vector<unsigned char>::const_iterator&) const;
    unsigned long long parseLength(std::vector<unsigned char>::const_iterator&) const;
    void switchContext(std::vector<unsigned char>::const_iterator&) const;
    void parseSequence(std::vector<unsigned char>::const_iterator&, int length) const;
    void parseInteger(std::vector<unsigned char>::const_iterator&, int length) const;
    void parseOctetString(std::vector<unsigned char>::const_iterator&, int length) const;
    void parseUnknown(std::vector<unsigned char>::const_iterator&, int length) const;
    void throwOnOutOfBounds(std::vector<unsigned char>::const_iterator&) const;
    const std::vector<unsigned char>& code;
  };
}
