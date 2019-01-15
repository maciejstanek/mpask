#pragma once

#include "mpask/DataSequenceElement.hxx"
#include "mpask/Address.hxx"

#include <string>
#include <vector>
#include <tuple>
#include <memory>

namespace mpask {

typedef std::vector<std::pair<Address, std::string>> vector_pair_Address_string;

enum class MessageType {
  GetRequest = 0,
  GetNextRequest = 1,
  GetResponse = 2,
  SetRequest = 3
};

struct Message
{
  explicit Message(MessageType);
  std::vector<unsigned char> encode() const;

  int version {1};
  std::string community;
  MessageType type;
  vector_pair_Address_string values;

private:
  std::vector<unsigned char> encodeString(const std::string& s) const;
};

}
