#include "mpask/MiniService.hxx"

#include "mpask/Message.hxx"
#include "mpask/Exception.hxx"
#include "mpask/MasterParser.hxx"

#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <iterator>     // std::ostream_iterator
#include <vector>       // std::vector
#include <algorithm>

using namespace std;

namespace mpask
{
  std::vector<unsigned char>
  MiniService::handleRequest(const vector<unsigned char>& v)
  {
    cerr << "DBG sysUpTime '" << getSysUpTime() << "'" << endl;
    cerr << "DBG sysName '" << getSysName() << "'" << endl;
    cerr << "DBG ipForwarding '" << getIpForwarding() << "'" << endl;

    Message m {v, MasterParser{}("resources", "RFC1213-MIB")};
    cerr << "DBG handleRequest type1 = " << static_cast<int>(m.type) << endl;

    if (m.type == MessageType::GetRequest) {
      m.type = MessageType::GetResponse;
      for (auto& [oid, val] : m.values) {
        cerr << "GET processing \"";
        copy(oid.begin(), oid.end(), ostream_iterator<int>(cerr, "."));
        cerr << "\".\n";
        if (oid == vector<int>{1,3,3,1,2,1,1,3}) {
          cerr << "GET it is sysUpTime" << endl;
          val = getSysUpTime();
        }
        else if (oid == vector<int>{1,3,6,1,2,1,1,5}) {
          cerr << "GET it is sysName" << endl;
          val = getSysName();
        }
        else if (oid == vector<int>{1,3,6,1,2,1,4,1}) {
          cerr << "GET it is ipForwarding" << endl;
          val = getIpForwarding();
        }
      }
    }
    else if (m.type == MessageType::SetRequest) {
      m.type = MessageType::GetResponse; // TODO: No "SetResponse"?
      throw Exception {"SetRequest not handled yet."};
    }
    else {
      throw Exception {"Only SetRequest and GetRequest are handled."};
    }
    cerr << "DBG handleRequest type2 = " << static_cast<int>(m.type) << endl;
    return m.encode();
  }

  string
  MiniService::getSysUpTime()
  {
    system(R"(sed 's/\([0-9]\+\).*/\1/' /proc/uptime > /home/mstanek/Studia/MPASK/lab/STATUS/sysUpTime)");
    ifstream s("/home/mstanek/Studia/MPASK/lab/STATUS/sysUpTime");
    string t;
    s >> t;
    return t;
  }

  string
  MiniService::getSysName()
  {
    system(R"(hostname > /home/mstanek/Studia/MPASK/lab/STATUS/sysName)");
    ifstream s("/home/mstanek/Studia/MPASK/lab/STATUS/sysName");
    string t;
    s >> t;
    return t;
  }

  string
  MiniService::getIpForwarding()
  {
    ifstream s("/home/mstanek/Studia/MPASK/lab/STATUS/ipForwarding");
    string t;
    s >> t;
    return t;
  }

  void
  MiniService::setIpForwarding(string x)
  {
    ofstream s("/home/mstanek/Studia/MPASK/lab/STATUS/ipForwarding", ofstream::trunc);
    s << x;
  }
}
