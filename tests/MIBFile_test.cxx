#include "mpask/MIBFile.hxx"

#include <gtest/gtest.h>

using namespace std;
using namespace mpask;

class MIBFile_test : public ::testing::Test
{
};

TEST_F(MIBFile_test, meld)
{
  MIBFile a;
  a.imports.push_back({{{"a"}}, "a"});
  SequenceDeclaration aSequence;
  aSequence.name = "a";
  a.sequences.push_back(aSequence);
  TypeDeclaration aType;
  aType.name = "a";
  a.types.push_back(aType);
  AliasDeclaration aAlias;
  aAlias.name = "a";
  a.aliases.push_back(aAlias);

  MIBFile b;
  b.imports.push_back({{{"b"}}, "b"});
  SequenceDeclaration bSequence;
  bSequence.name = "b";
  b.sequences.push_back(bSequence);
  TypeDeclaration bType;
  bType.name = "b";
  b.types.push_back(bType);
  AliasDeclaration bAlias;
  bAlias.name = "b";
  b.aliases.push_back(bAlias);

  a.meld(b);

  int importsCount {};
  for (const auto& import : a.imports) {
    switch (importsCount++) {
      case 0:
        EXPECT_EQ(import.first.at(0), "a"s);
        EXPECT_EQ(import.second, "a"s);
        break;
      case 1:
        EXPECT_EQ(import.first.at(0), "b"s);
        EXPECT_EQ(import.second, "b"s);
        break;
    }
  }
  EXPECT_EQ(importsCount, 2);

  int sequencesCount {};
  for (const auto& sequence : a.sequences) {
    switch (sequencesCount++) {
      case 0:
        EXPECT_EQ(sequence.name, "a"s);
        break;
      case 1:
        EXPECT_EQ(sequence.name, "b"s);
        break;
    }
  }
  EXPECT_EQ(sequencesCount, 2);

  int typesCount {};
  for (const auto& type : a.types) {
    switch (typesCount++) {
      case 0:
        EXPECT_EQ(type.name, "a"s);
        break;
      case 1:
        EXPECT_EQ(type.name, "b"s);
        break;
    }
  }
  EXPECT_EQ(typesCount, 2);

  int aliasesCount {};
  for (const auto& alias : a.aliases) {
    switch (aliasesCount++) {
      case 0:
        EXPECT_EQ(alias.name, "a"s);
        break;
      case 1:
        EXPECT_EQ(alias.name, "b"s);
        break;
    }
  }
  EXPECT_EQ(aliasesCount, 2);
}
