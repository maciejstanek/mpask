#pragma once

#include "mpask/DataSequenceElement.hxx"
#include "mpask/MIBFile.hxx"

#include <string>
#include <vector>
#include <tuple>
#include <memory>

namespace mpask {

typedef std::vector<std::pair<std::vector<int>, std::string>> vector_pair_vector_int_string;

enum class MessageType {
  GetRequest = 0,
  GetNextRequest = 1,
  GetResponse = 2,
  SetRequest = 3
};

struct Message
{
  std::vector<unsigned char> encode() const;
  Message(const std::vector<unsigned char>&, const std::shared_ptr<MIBFile>&);
  Message(const vector_pair_vector_int_string&, const std::shared_ptr<MIBFile>&);

  int version {1};
  std::string community {"private"};
  MessageType type {MessageType::GetRequest};
  vector_pair_vector_int_string values;
  std::shared_ptr<MIBFile> schema;

private:
  std::vector<unsigned char> encodeString(const std::string& s) const;
};

bool operator==(const Message&, const Message&);

}
