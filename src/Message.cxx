#include "mpask/Message.hxx"

#include "mpask/LengthKober.hxx"
#include "mpask/ObjectIdentifierKober.hxx"

#include <iostream>
#include <iomanip>

using namespace std;

namespace mpask
{
  vector<unsigned char>
  Message::encode() const
  {
    // SNMP Message
    vector<unsigned char> code = {0x30};
    int len = 0;

    // SNMP Version
    vector<unsigned char> versionCode = {0x02, 0x01, static_cast<unsigned char>(version)};
    len += versionCode.size();

    // SNMP Community String
    vector<unsigned char> communityCode = encodeString(community);
    len += communityCode.size();
    
    // SNMP PDU
    vector<unsigned char> unitCode = {0xa0};
    int unitLength = 0;

    // Request ID
    vector<unsigned char> requestCode = {0x02, 0x01, 0x01};
    unitLength += requestCode.size();

    // Error
    vector<unsigned char> errorCode = {0x02, 0x01, 0x00};
    unitLength += errorCode.size();

    // Error Index
    vector<unsigned char> indexCode = {0x02, 0x01, 0x00};
    unitLength += indexCode.size();

    // Varbind List
    vector<unsigned char> varbindListCode = {0x30};
    int varbindListCodeLen = 0;
    vector<unsigned char> varbindListValueCode = {};
    for (const auto& valuePair : values) {
      auto identifier = valuePair.first;
      auto value = valuePair.second;

      // Varbind
      vector<unsigned char> varbindCode = {0x30};
      int varbindCodeLen = 0;

      // Object Identifier
      vector<unsigned char> identifierCode = ObjectIdentifierKober()(identifier);
      varbindCodeLen += identifierCode.size();
#if 0
      cerr << "DBG: identifierCode ->";
      for (auto c : identifierCode) {
        cerr << " 0x" << hex << setfill('0') << setw(2) << static_cast<int>(c);
      }
      cerr << "\n";
#endif

      // Value
      vector<unsigned char> valueCode = encodeString(value); // TODO: encode other types too
      varbindCodeLen += valueCode.size();

      // Varbind (continued)
      auto varbindCodeLenCode = LengthKober()(varbindCodeLen);
      varbindCode.insert(varbindCode.end(), varbindCodeLenCode.begin(), varbindCodeLenCode.end());
      varbindCode.insert(varbindCode.end(), identifierCode.begin(), identifierCode.end());
      varbindCode.insert(varbindCode.end(), valueCode.begin(), valueCode.end());

      // Varbind List (continued)
      varbindListValueCode.insert(varbindListValueCode.end(), varbindCode.begin(), varbindCode.end());
    }

    // Varbind List (continued)
    varbindListCodeLen += varbindListValueCode.size();
    auto varbindListCodeLenCode = LengthKober()(varbindListCodeLen);
    varbindListCode.insert(varbindListCode.end(), varbindListCodeLenCode.begin(), varbindListCodeLenCode.end());
    varbindListCode.insert(varbindListCode.end(), varbindListValueCode.begin(), varbindListValueCode.end());
    unitLength += varbindListCodeLen;

    // SNMP PDU (continued)
    auto unitLengthCode = LengthKober()(unitLength);
    unitCode.insert(unitCode.end(), unitLengthCode.begin(), unitLengthCode.end());
    unitCode.insert(unitCode.end(), requestCode.begin(), requestCode.end());
    unitCode.insert(unitCode.end(), errorCode.begin(), errorCode.end());
    unitCode.insert(unitCode.end(), indexCode.begin(), indexCode.end());
    unitCode.insert(unitCode.end(), varbindListCode.begin(), varbindListCode.end());
    len += unitCode.size();

    // SNMP Message
    auto lenCode = LengthKober()(len);
    code.insert(code.end(), lenCode.begin(), lenCode.end());
    code.insert(code.end(), versionCode.begin(), versionCode.end());
    code.insert(code.end(), communityCode.begin(), communityCode.end());
    code.insert(code.end(), unitCode.begin(), unitCode.end());

    return code;
  }

  vector<unsigned char>
  Message::encodeString(const string& s) const
  {
    vector<unsigned char> code = {0x04};
    auto lencode = LengthKober()(s.size());
    code.insert(code.end(), lencode.begin(), lencode.end());
    for (auto c : s) {
      code.push_back(c);
    }
#if 0
    cerr << "DBG: encodeString(\"" << s << "\") ->";
    for (auto c : code) {
      cerr << " 0x" << hex << setfill('0') << setw(2) << static_cast<int>(c);
    }
    cerr << "\n";
#endif
    return code;
  }
}
