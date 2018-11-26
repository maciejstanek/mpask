#include "mpask/TreeBuilder.hxx"

#include "mpask/Parser.hxx"
#include "mpask/MasterParser.hxx"
#include "mpask/Exception.hxx"

#include <gtest/gtest.h>
#include <sstream>
#include <fstream>

using namespace std;
using namespace mpask;

class TreeBuilder_test : public ::testing::Test
{
};

TEST_F(TreeBuilder_test, example_1)
{
  stringstream input {R"(
    x DEFINITIONS ::= BEGIN
      xxx OBJECT IDENTIFIER ::= { iso 123 }
      aaa OBJECT IDENTIFIER ::= { xxx 1 }
      bbb OBJECT IDENTIFIER ::= { xxx 2 }
      ccc OBJECT IDENTIFIER ::= { xxx 3 }
      ddd OBJECT IDENTIFIER ::= { bbb 1 }
      eee OBJECT IDENTIFIER ::= { bbb 2 }
      fff OBJECT IDENTIFIER ::= { bbb 3 }
      ggg OBJECT IDENTIFIER ::= { eee 1 }
      hhh OBJECT IDENTIFIER ::= { eee 2 }
      iii OBJECT IDENTIFIER ::= { eee 3 }
    END
  )"};
  auto object = Parser{}(input);
  auto root = TreeBuilder{}(object);

  stringstream output;
  root->printHierarchy(output);
  EXPECT_EQ(output.str(),
		"root()\n"
		"  iso(1)\n"
		"    xxx(1.123)\n"
		"      aaa(1.123.1)\n"
		"      bbb(1.123.2)\n"
		"        ddd(1.123.2.1)\n"
		"        eee(1.123.2.2)\n"
		"          ggg(1.123.2.2.1)\n"
		"          hhh(1.123.2.2.2)\n"
		"          iii(1.123.2.2.3)\n"
		"        fff(1.123.2.3)\n"
		"      ccc(1.123.3)\n"
	);

  ASSERT_EQ(root->getName(), "root"s); // NOTE: Always "root".
  ASSERT_EQ(root->size(), 1);
  auto& iso = root->begin()->second;
  ASSERT_EQ(iso->getName(), "iso"s);
  ASSERT_EQ(iso->size(), 1);
  auto& xxx = iso->begin()->second;
  ASSERT_EQ(xxx->getName(), "xxx"s);
  ASSERT_EQ(xxx->size(), 3);
  int nodeIter {};
  shared_ptr<Node> sub;
  for (const auto& [identifier, node] : *xxx) {
    switch (++nodeIter) {
      case 1:
        EXPECT_EQ(node->getName(), "aaa"s);
        EXPECT_EQ(node->getIdentifier(), 1);
        EXPECT_EQ(identifier, 1);
        EXPECT_EQ(node->size(), 0);
        break;
      case 2:
        EXPECT_EQ(node->getName(), "bbb"s);
        EXPECT_EQ(node->getIdentifier(), 2);
        EXPECT_EQ(identifier, 2);
        EXPECT_EQ(node->size(), 3);
        sub = node;
        break;
      case 3:
        EXPECT_EQ(node->getName(), "ccc"s);
        EXPECT_EQ(node->getIdentifier(), 3);
        EXPECT_EQ(identifier, 3);
        EXPECT_EQ(node->size(), 0);
        break;
    }
  }
  EXPECT_EQ(nodeIter, 3);
  ASSERT_NE(sub, nullptr);

  int subsubIter {};
  for (const auto& [identifier, subsub] : *sub) {
    switch (++subsubIter) {
      case 1:
        EXPECT_EQ(subsub->getName(), "ddd"s);
        EXPECT_EQ(subsub->getIdentifier(), 1);
        EXPECT_EQ(identifier, 1);
        EXPECT_EQ(subsub->size(), 0);
        break;
      case 2:
        EXPECT_EQ(subsub->getName(), "eee"s);
        EXPECT_EQ(subsub->getIdentifier(), 2);
        EXPECT_EQ(identifier, 2);
        EXPECT_EQ(subsub->size(), 3);
        break;
      case 3:
        EXPECT_EQ(subsub->getName(), "fff"s);
        EXPECT_EQ(subsub->getIdentifier(), 3);
        EXPECT_EQ(identifier, 3);
        EXPECT_EQ(subsub->size(), 0);
        break;
    }
  }
  EXPECT_EQ(subsubIter, 3);
}

TEST_F(TreeBuilder_test, mib)
{
  auto object = MasterParser{}("resources", "RFC1213-MIB");
  auto root = TreeBuilder{}(object);
  stringstream output;
  root->printHierarchy(output);
  EXPECT_EQ(output.str(),
    "root()\n"
    "  iso(1)\n"
    "    org(1.3)\n"
    "      dod(1.3.6)\n"
    "        internet(1.3.6.1)\n"
    "          directory(1.3.6.1.1)\n"
    "          mgmt(1.3.6.1.2)\n"
    "            mib-2(1.3.6.1.2.1)\n"
    "              system(1.3.6.1.2.1.1)\n"
    "                sysDescr(1.3.6.1.2.1.1.1)\n"
    "                sysObjectID(1.3.6.1.2.1.1.2)\n"
    "                sysUpTime(1.3.6.1.2.1.1.3)\n"
    "                sysContact(1.3.6.1.2.1.1.4)\n"
    "                sysName(1.3.6.1.2.1.1.5)\n"
    "                sysLocation(1.3.6.1.2.1.1.6)\n"
    "                sysServices(1.3.6.1.2.1.1.7)\n"
    "              interfaces(1.3.6.1.2.1.2)\n"
    "                ifNumber(1.3.6.1.2.1.2.1)\n"
    "                ifTable(1.3.6.1.2.1.2.2)\n"
    "                  ifEntry(1.3.6.1.2.1.2.2.1)\n"
    "                    ifIndex(1.3.6.1.2.1.2.2.1.1)\n"
    "                    ifDescr(1.3.6.1.2.1.2.2.1.2)\n"
    "                    ifType(1.3.6.1.2.1.2.2.1.3)\n"
    "                    ifMtu(1.3.6.1.2.1.2.2.1.4)\n"
    "                    ifSpeed(1.3.6.1.2.1.2.2.1.5)\n"
    "                    ifPhysAddress(1.3.6.1.2.1.2.2.1.6)\n"
    "                    ifAdminStatus(1.3.6.1.2.1.2.2.1.7)\n"
    "                    ifOperStatus(1.3.6.1.2.1.2.2.1.8)\n"
    "                    ifLastChange(1.3.6.1.2.1.2.2.1.9)\n"
    "                    ifInOctets(1.3.6.1.2.1.2.2.1.10)\n"
    "                    ifInUcastPkts(1.3.6.1.2.1.2.2.1.11)\n"
    "                    ifInNUcastPkts(1.3.6.1.2.1.2.2.1.12)\n"
    "                    ifInDiscards(1.3.6.1.2.1.2.2.1.13)\n"
    "                    ifInErrors(1.3.6.1.2.1.2.2.1.14)\n"
    "                    ifInUnknownProtos(1.3.6.1.2.1.2.2.1.15)\n"
    "                    ifOutOctets(1.3.6.1.2.1.2.2.1.16)\n"
    "                    ifOutUcastPkts(1.3.6.1.2.1.2.2.1.17)\n"
    "                    ifOutNUcastPkts(1.3.6.1.2.1.2.2.1.18)\n"
    "                    ifOutDiscards(1.3.6.1.2.1.2.2.1.19)\n"
    "                    ifOutErrors(1.3.6.1.2.1.2.2.1.20)\n"
    "                    ifOutQLen(1.3.6.1.2.1.2.2.1.21)\n"
    "                    ifSpecific(1.3.6.1.2.1.2.2.1.22)\n"
    "              at(1.3.6.1.2.1.3)\n"
    "                atTable(1.3.6.1.2.1.3.1)\n"
    "                  atEntry(1.3.6.1.2.1.3.1.1)\n"
    "                    atIfIndex(1.3.6.1.2.1.3.1.1.1)\n"
    "                    atPhysAddress(1.3.6.1.2.1.3.1.1.2)\n"
    "                    atNetAddress(1.3.6.1.2.1.3.1.1.3)\n"
    "              ip(1.3.6.1.2.1.4)\n"
    "                ipForwarding(1.3.6.1.2.1.4.1)\n"
    "                ipDefaultTTL(1.3.6.1.2.1.4.2)\n"
    "                ipInReceives(1.3.6.1.2.1.4.3)\n"
    "                ipInHdrErrors(1.3.6.1.2.1.4.4)\n"
    "                ipInAddrErrors(1.3.6.1.2.1.4.5)\n"
    "                ipForwDatagrams(1.3.6.1.2.1.4.6)\n"
    "                ipInUnknownProtos(1.3.6.1.2.1.4.7)\n"
    "                ipInDiscards(1.3.6.1.2.1.4.8)\n"
    "                ipInDelivers(1.3.6.1.2.1.4.9)\n"
    "                ipOutRequests(1.3.6.1.2.1.4.10)\n"
    "                ipOutDiscards(1.3.6.1.2.1.4.11)\n"
    "                ipOutNoRoutes(1.3.6.1.2.1.4.12)\n"
    "                ipReasmTimeout(1.3.6.1.2.1.4.13)\n"
    "                ipReasmReqds(1.3.6.1.2.1.4.14)\n"
    "                ipReasmOKs(1.3.6.1.2.1.4.15)\n"
    "                ipReasmFails(1.3.6.1.2.1.4.16)\n"
    "                ipFragOKs(1.3.6.1.2.1.4.17)\n"
    "                ipFragFails(1.3.6.1.2.1.4.18)\n"
    "                ipFragCreates(1.3.6.1.2.1.4.19)\n"
    "                ipAddrTable(1.3.6.1.2.1.4.20)\n"
    "                  ipAddrEntry(1.3.6.1.2.1.4.20.1)\n"
    "                    ipAdEntAddr(1.3.6.1.2.1.4.20.1.1)\n"
    "                    ipAdEntIfIndex(1.3.6.1.2.1.4.20.1.2)\n"
    "                    ipAdEntNetMask(1.3.6.1.2.1.4.20.1.3)\n"
    "                    ipAdEntBcastAddr(1.3.6.1.2.1.4.20.1.4)\n"
    "                    ipAdEntReasmMaxSize(1.3.6.1.2.1.4.20.1.5)\n"
    "                ipRouteTable(1.3.6.1.2.1.4.21)\n"
    "                  ipRouteEntry(1.3.6.1.2.1.4.21.1)\n"
    "                    ipRouteDest(1.3.6.1.2.1.4.21.1.1)\n"
    "                    ipRouteIfIndex(1.3.6.1.2.1.4.21.1.2)\n"
    "                    ipRouteMetric1(1.3.6.1.2.1.4.21.1.3)\n"
    "                    ipRouteMetric2(1.3.6.1.2.1.4.21.1.4)\n"
    "                    ipRouteMetric3(1.3.6.1.2.1.4.21.1.5)\n"
    "                    ipRouteMetric4(1.3.6.1.2.1.4.21.1.6)\n"
    "                    ipRouteNextHop(1.3.6.1.2.1.4.21.1.7)\n"
    "                    ipRouteType(1.3.6.1.2.1.4.21.1.8)\n"
    "                    ipRouteProto(1.3.6.1.2.1.4.21.1.9)\n"
    "                    ipRouteAge(1.3.6.1.2.1.4.21.1.10)\n"
    "                    ipRouteMask(1.3.6.1.2.1.4.21.1.11)\n"
    "                    ipRouteMetric5(1.3.6.1.2.1.4.21.1.12)\n"
    "                    ipRouteInfo(1.3.6.1.2.1.4.21.1.13)\n"
    "                ipNetToMediaTable(1.3.6.1.2.1.4.22)\n"
    "                  ipNetToMediaEntry(1.3.6.1.2.1.4.22.1)\n"
    "                    ipNetToMediaIfIndex(1.3.6.1.2.1.4.22.1.1)\n"
    "                    ipNetToMediaPhysAddress(1.3.6.1.2.1.4.22.1.2)\n"
    "                    ipNetToMediaNetAddress(1.3.6.1.2.1.4.22.1.3)\n"
    "                    ipNetToMediaType(1.3.6.1.2.1.4.22.1.4)\n"
    "                ipRoutingDiscards(1.3.6.1.2.1.4.23)\n"
    "              icmp(1.3.6.1.2.1.5)\n"
    "                icmpInMsgs(1.3.6.1.2.1.5.1)\n"
    "                icmpInErrors(1.3.6.1.2.1.5.2)\n"
    "                icmpInDestUnreachs(1.3.6.1.2.1.5.3)\n"
    "                icmpInTimeExcds(1.3.6.1.2.1.5.4)\n"
    "                icmpInParmProbs(1.3.6.1.2.1.5.5)\n"
    "                icmpInSrcQuenchs(1.3.6.1.2.1.5.6)\n"
    "                icmpInRedirects(1.3.6.1.2.1.5.7)\n"
    "                icmpInEchos(1.3.6.1.2.1.5.8)\n"
    "                icmpInEchoReps(1.3.6.1.2.1.5.9)\n"
    "                icmpInTimestamps(1.3.6.1.2.1.5.10)\n"
    "                icmpInTimestampReps(1.3.6.1.2.1.5.11)\n"
    "                icmpInAddrMasks(1.3.6.1.2.1.5.12)\n"
    "                icmpInAddrMaskReps(1.3.6.1.2.1.5.13)\n"
    "                icmpOutMsgs(1.3.6.1.2.1.5.14)\n"
    "                icmpOutErrors(1.3.6.1.2.1.5.15)\n"
    "                icmpOutDestUnreachs(1.3.6.1.2.1.5.16)\n"
    "                icmpOutTimeExcds(1.3.6.1.2.1.5.17)\n"
    "                icmpOutParmProbs(1.3.6.1.2.1.5.18)\n"
    "                icmpOutSrcQuenchs(1.3.6.1.2.1.5.19)\n"
    "                icmpOutRedirects(1.3.6.1.2.1.5.20)\n"
    "                icmpOutEchos(1.3.6.1.2.1.5.21)\n"
    "                icmpOutEchoReps(1.3.6.1.2.1.5.22)\n"
    "                icmpOutTimestamps(1.3.6.1.2.1.5.23)\n"
    "                icmpOutTimestampReps(1.3.6.1.2.1.5.24)\n"
    "                icmpOutAddrMasks(1.3.6.1.2.1.5.25)\n"
    "                icmpOutAddrMaskReps(1.3.6.1.2.1.5.26)\n"
    "              tcp(1.3.6.1.2.1.6)\n"
    "                tcpRtoAlgorithm(1.3.6.1.2.1.6.1)\n"
    "                tcpRtoMin(1.3.6.1.2.1.6.2)\n"
    "                tcpRtoMax(1.3.6.1.2.1.6.3)\n"
    "                tcpMaxConn(1.3.6.1.2.1.6.4)\n"
    "                tcpActiveOpens(1.3.6.1.2.1.6.5)\n"
    "                tcpPassiveOpens(1.3.6.1.2.1.6.6)\n"
    "                tcpAttemptFails(1.3.6.1.2.1.6.7)\n"
    "                tcpEstabResets(1.3.6.1.2.1.6.8)\n"
    "                tcpCurrEstab(1.3.6.1.2.1.6.9)\n"
    "                tcpInSegs(1.3.6.1.2.1.6.10)\n"
    "                tcpOutSegs(1.3.6.1.2.1.6.11)\n"
    "                tcpRetransSegs(1.3.6.1.2.1.6.12)\n"
    "                tcpConnTable(1.3.6.1.2.1.6.13)\n"
    "                  tcpConnEntry(1.3.6.1.2.1.6.13.1)\n"
    "                    tcpConnState(1.3.6.1.2.1.6.13.1.1)\n"
    "                    tcpConnLocalAddress(1.3.6.1.2.1.6.13.1.2)\n"
    "                    tcpConnLocalPort(1.3.6.1.2.1.6.13.1.3)\n"
    "                    tcpConnRemAddress(1.3.6.1.2.1.6.13.1.4)\n"
    "                    tcpConnRemPort(1.3.6.1.2.1.6.13.1.5)\n"
    "                tcpInErrs(1.3.6.1.2.1.6.14)\n"
    "                tcpOutRsts(1.3.6.1.2.1.6.15)\n"
    "              udp(1.3.6.1.2.1.7)\n"
    "                udpInDatagrams(1.3.6.1.2.1.7.1)\n"
    "                udpNoPorts(1.3.6.1.2.1.7.2)\n"
    "                udpInErrors(1.3.6.1.2.1.7.3)\n"
    "                udpOutDatagrams(1.3.6.1.2.1.7.4)\n"
    "                udpTable(1.3.6.1.2.1.7.5)\n"
    "                  udpEntry(1.3.6.1.2.1.7.5.1)\n"
    "                    udpLocalAddress(1.3.6.1.2.1.7.5.1.1)\n"
    "                    udpLocalPort(1.3.6.1.2.1.7.5.1.2)\n"
    "              egp(1.3.6.1.2.1.8)\n"
    "                egpInMsgs(1.3.6.1.2.1.8.1)\n"
    "                egpInErrors(1.3.6.1.2.1.8.2)\n"
    "                egpOutMsgs(1.3.6.1.2.1.8.3)\n"
    "                egpOutErrors(1.3.6.1.2.1.8.4)\n"
    "                egpNeighTable(1.3.6.1.2.1.8.5)\n"
    "                  egpNeighEntry(1.3.6.1.2.1.8.5.1)\n"
    "                    egpNeighState(1.3.6.1.2.1.8.5.1.1)\n"
    "                    egpNeighAddr(1.3.6.1.2.1.8.5.1.2)\n"
    "                    egpNeighAs(1.3.6.1.2.1.8.5.1.3)\n"
    "                    egpNeighInMsgs(1.3.6.1.2.1.8.5.1.4)\n"
    "                    egpNeighInErrs(1.3.6.1.2.1.8.5.1.5)\n"
    "                    egpNeighOutMsgs(1.3.6.1.2.1.8.5.1.6)\n"
    "                    egpNeighOutErrs(1.3.6.1.2.1.8.5.1.7)\n"
    "                    egpNeighInErrMsgs(1.3.6.1.2.1.8.5.1.8)\n"
    "                    egpNeighOutErrMsgs(1.3.6.1.2.1.8.5.1.9)\n"
    "                    egpNeighStateUps(1.3.6.1.2.1.8.5.1.10)\n"
    "                    egpNeighStateDowns(1.3.6.1.2.1.8.5.1.11)\n"
    "                    egpNeighIntervalHello(1.3.6.1.2.1.8.5.1.12)\n"
    "                    egpNeighIntervalPoll(1.3.6.1.2.1.8.5.1.13)\n"
    "                    egpNeighMode(1.3.6.1.2.1.8.5.1.14)\n"
    "                    egpNeighEventTrigger(1.3.6.1.2.1.8.5.1.15)\n"
    "                egpAs(1.3.6.1.2.1.8.6)\n"
    "              transmission(1.3.6.1.2.1.10)\n"
    "              snmp(1.3.6.1.2.1.11)\n"
    "                snmpInPkts(1.3.6.1.2.1.11.1)\n"
    "                snmpOutPkts(1.3.6.1.2.1.11.2)\n"
    "                snmpInBadVersions(1.3.6.1.2.1.11.3)\n"
    "                snmpInBadCommunityNames(1.3.6.1.2.1.11.4)\n"
    "                snmpInBadCommunityUses(1.3.6.1.2.1.11.5)\n"
    "                snmpInASNParseErrs(1.3.6.1.2.1.11.6)\n"
    "                snmpInTooBigs(1.3.6.1.2.1.11.8)\n"
    "                snmpInNoSuchNames(1.3.6.1.2.1.11.9)\n"
    "                snmpInBadValues(1.3.6.1.2.1.11.10)\n"
    "                snmpInReadOnlys(1.3.6.1.2.1.11.11)\n"
    "                snmpInGenErrs(1.3.6.1.2.1.11.12)\n"
    "                snmpInTotalReqVars(1.3.6.1.2.1.11.13)\n"
    "                snmpInTotalSetVars(1.3.6.1.2.1.11.14)\n"
    "                snmpInGetRequests(1.3.6.1.2.1.11.15)\n"
    "                snmpInGetNexts(1.3.6.1.2.1.11.16)\n"
    "                snmpInSetRequests(1.3.6.1.2.1.11.17)\n"
    "                snmpInGetResponses(1.3.6.1.2.1.11.18)\n"
    "                snmpInTraps(1.3.6.1.2.1.11.19)\n"
    "                snmpOutTooBigs(1.3.6.1.2.1.11.20)\n"
    "                snmpOutNoSuchNames(1.3.6.1.2.1.11.21)\n"
    "                snmpOutBadValues(1.3.6.1.2.1.11.22)\n"
    "                snmpOutGenErrs(1.3.6.1.2.1.11.24)\n"
    "                snmpOutGetRequests(1.3.6.1.2.1.11.25)\n"
    "                snmpOutGetNexts(1.3.6.1.2.1.11.26)\n"
    "                snmpOutSetRequests(1.3.6.1.2.1.11.27)\n"
    "                snmpOutGetResponses(1.3.6.1.2.1.11.28)\n"
    "                snmpOutTraps(1.3.6.1.2.1.11.29)\n"
    "                snmpEnableAuthenTraps(1.3.6.1.2.1.11.30)\n"
    "          experimental(1.3.6.1.3)\n"
    "          private(1.3.6.1.4)\n"
    "            enterprises(1.3.6.1.4.1)\n"
  );
  ofstream dot;
  dot.open("mib.dot");
  root->printDotFile(dot);
  dot.close();
}
