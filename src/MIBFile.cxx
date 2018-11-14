#include "mpask/MIBFile.hxx"

using namespace std;

namespace mpask
{
  void
  MIBFile::meld(const MIBFile& dissolved)
  {
    imports.insert(imports.end(), dissolved.imports.begin(), dissolved.imports.end());
    sequences.insert(sequences.end(), dissolved.sequences.begin(), dissolved.sequences.end());
    types.insert(types.end(), dissolved.types.begin(), dissolved.types.end());
    aliases.insert(aliases.end(), dissolved.aliases.begin(), dissolved.aliases.end());
  }
}
