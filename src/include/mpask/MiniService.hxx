#pragma once

#include <vector>
#include <string>

namespace mpask {

class MiniService
{
  public:
    std::vector<unsigned char> handleRequest(const std::vector<unsigned char>&);
  private:
    std::string getSysUpTime();
    std::string getSysName();
    std::string getIpForwarding();
    void setIpForwarding(std::string);
};

}
