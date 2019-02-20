#include "mpask/Message.hxx"

#include "mpask/LengthKober.hxx"
#include "mpask/Exception.hxx"
#include "mpask/ObjectIdentifierKober.hxx"
#include "mpask/Address.hxx"
#include "mpask/TreeBuilder.hxx"
#include "mpask/ValueKober.hxx"
#include "mpask/DataValue.hxx"
#include "mpask/AliasDeclarationGrammar.hxx"
#include "mpask/AliasDeclaration.hxx"
#include "mpask/ObjectIdentifierDekober.hxx"
#include "mpask/Dekober.hxx"

#include <boost/spirit/include/qi.hpp>
#include <iostream>
#include <iomanip>

using namespace std;

namespace mpask
{
  Message::Message
    ( const std::vector<unsigned char>& code
    , const std::shared_ptr<MIBFile>& schema_
    )
    : schema {schema_}
  {
    cerr << "parsing msg";
    for (auto c : code) {
      cerr << " 0x" << hex << setfill('0') << setw(2) << static_cast<int>(c);
    }
    cerr << endl;
    // NOTE: This decoder is written in a very dirty fashion. I assume, that
    // the sequence will contain at least 3 0x30 values (sequences), and two of
    // them for most of the cases will be separated by a one byte. I just
    // for this third 0x30 byte and discard everything before.
    [[maybe_unused]] auto v1 = find(code.begin(), code.end(), 0x30);
    auto v2 = find(v1 + 1, code.end(), 0x30);
    auto v3 = find(v2 + 1, code.end(), 0x30);
    if (v3 - v2 != 2 || v3 == code.end()) {
      throw Exception {"DECODER FATAL ERROR"};
    }
    cerr << "DBG SLICE";
    for (auto i = v3; i != code.end(); ++i) {
      cerr << " 0x" << hex << setfill('0') << setw(2) << static_cast<int>(*i);
    }
    cerr << endl;

    auto i = v3;
    cerr << "WARNING: assuming one byte length in the decoding process." << endl;
    auto root = TreeBuilder{}(schema);
    root->printHierarchy(cerr);
    while (i != code.end()) {
      ++i; // skip sequence header
      ++i; // skip total length
      auto l = static_cast<int>(*(i + 1)) + 2; // one byte length
      vector<unsigned char> rawoid(i, i + l);
      cerr << "DBG raw oid";
      for (auto r : rawoid) {
        cerr << " 0x" << hex << setfill('0') << setw(2) << static_cast<int>(r);
      }
      cerr << endl;
      auto oid = ObjectIdentifierDekober().getVector(rawoid);
      cerr << "DBG decoded OID";
      for (auto a : oid) {
        cerr << " " << a;
      }
      cerr << endl;

      TypeDeclaration d;
      try {
        auto x = root->findNodeByOID(oid);
        d = x->getSource();
      }
      catch (Exception& e) {
        cerr << "Error while searching for OID \"";
        copy(oid.begin(), oid.end(), ostream_iterator<int>(cerr,"."));
        cerr << "\".\n";
        cerr << e.what() << "\n";
        cerr << "Assuming OCTET STRING type.\n";
        d.baseType.name = "OBJECT-TYPE";
        d.syntax.name.name = "OCTET STRING";
      }
      cerr << "DBG data type = " << d << endl;

      i += l;
      auto lv = static_cast<int>(*(i + 1)) + 2; // one byte length
      vector<unsigned char> rawv(i, i + lv);
      cerr << "DBG raw value";
      for (auto r : rawv) {
        cerr << " 0x" << hex << setfill('0') << setw(2) << static_cast<int>(r);
      }
      cerr << endl;
      i += lv;
      auto result = Dekober(rawv)();
      cerr << "DBG result = " << *result << endl;
      string stringval;
      string stringtag;
      if (auto val = dynamic_pointer_cast<DataValue>(result)) {
        stringval = val->value;
        stringtag = val->type;
      }
      else {
        throw Exception {"Can only parse values at the moment, not sequences."};
      }
      cerr << "DBG value = " << stringval << endl;

      if (d.syntax.name.name != stringtag) {
        throw Exception {"OID suggest type \""s + d.syntax.name.name + "\" but the raw type has tag \"" + stringtag + "\"."};
      }
      values.push_back({oid, stringval});
    }
  }

  Message::Message
    ( const vector_pair_vector_int_string& values_
    , const std::shared_ptr<MIBFile>& schema_
    )
    : values {values_}
    , schema {schema_}
  {
  }

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

    // SCHEMA: generate a tree from schema
    auto root = TreeBuilder{}(schema);
    root->printHierarchy(cerr);

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

      // SCHEMA: find the value (assume string if not found)
      TypeDeclaration d;
      try {
        auto x = root->findNodeByOID(identifier);
        d = x->getSource();
      }
      catch (Exception& e) {
        cerr << "Error while searching for OID \"";
        copy(identifier.begin(), identifier.end(), ostream_iterator<int>(cerr,"."));
        cerr << "\".\n";
        cerr << e.what() << "\n";
        cerr << "Assuming OCTET STRING type.\n";
        d.baseType.name = "OBJECT-TYPE";
        d.syntax.name.name = "OCTET STRING";
      }

      ////////////////////////////////////////////////


      string input {"xxx ::= "s + d.syntax.name.name};
      string::const_iterator iter = input.begin();
      string::const_iterator end = input.end();
      AliasDeclarationGrammar<string::const_iterator> parser;
      boost::spirit::ascii::space_type space;
      AliasDeclaration result;
      bool status = phrase_parse(iter, end, parser, space, result);
      if (!status) {
        throw Exception {"err"};
      }
      // cerr << result.dataType << endl;
      auto data = make_shared<DataValue>();
      data->setContextAlias(result);
      data->setValue(value);
      auto coded = ValueKober()(data);

      ////////////////////////////////////////////////

      // Value
      vector<unsigned char> valueCode = ValueKober()(data); //encodeString(value); // TODO: encode other types too
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

  void
  Message::validate() const
  {
    auto root = TreeBuilder{}(schema);
    cerr << "VALIDATION" << endl;

    for (const auto& valuePair : values) {
      auto oid = valuePair.first;
      auto value = valuePair.second;

      cerr << "DBG OID: " << oid << endl << "DBG VAL: " << value << endl;

      TypeDeclaration d;
      try {
        auto x = root->findNodeByOID(oid);
        d = x->getSource();
      }
      catch (Exception& e) {
        cerr << "Error while searching for OID \"";
        copy(oid.begin(), oid.end(), ostream_iterator<int>(cerr,"."));
        cerr << "\".\n";
        cerr << e.what() << "\n";
        cerr << "Assuming OCTET STRING type.\n";
        d.baseType.name = "OBJECT-TYPE";
        d.syntax.name.name = "OCTET STRING";
      }
      cerr << "DBG data type = " << d << endl;

      if (d.syntax.name.name == "OCTET STRING" && d.syntax.restriction.size) {
        cerr << "VALIDATING OCTET STRING FOR SIZE" << endl;
        int maxs = max(d.syntax.restriction.left, d.syntax.restriction.right);
        int mins = min(d.syntax.restriction.left, d.syntax.restriction.right);
        if (static_cast<int>(value.size()) > maxs || static_cast<int>(value.size()) < mins) {
          ostringstream s;
          s << "Object ";
          copy(oid.begin(), oid.end(), ostream_iterator<int>(s,"."));
          s << " of type " << d.syntax.name.name << " does not fit in the allowed"
            << " size (" << value.size() << " is not in <" << mins << ", "
            << maxs << ">).";
          throw Exception {s.str()};
        }
      }

      if (d.syntax.name.name == "INTEGER" && d.syntax.restriction.range) {
        cerr << "VALIDATING INTEGER FOR RANGE" << endl;
        int maxs = max(d.syntax.restriction.left, d.syntax.restriction.right);
        int mins = min(d.syntax.restriction.left, d.syntax.restriction.right);
        if (stoi(value) > maxs || stoi(value) < mins) {
          ostringstream s;
          s << "Object ";
          copy(oid.begin(), oid.end(), ostream_iterator<int>(s,"."));
          s << " of type " << d.syntax.name.name << " does not fit in the allowed"
            << " range of values (" << value << " is not in <" << mins << ", "
            << maxs << ">).";
          throw Exception {s.str()};
        }
      }
    }
  }

  bool
  operator==(const Message& lhs, const Message& rhs)
  {
    if (lhs.version != rhs.version) {
      return false;
    }
    if (lhs.community != rhs.community) {
      return false;
    }
    if (lhs.type != rhs.type) {
      return false;
    }
    if (lhs.values != rhs.values) {
      return false;
    }
    return true;
  }
}
