#include "mpask/TreeBuilder.hxx"

#include "mpask/Exception.hxx"

using namespace std;

namespace mpask
{
  shared_ptr<Node>
  TreeBuilder::operator()(const MIBFile& input) const
  {
    return make_shared<Node>("", 0);
  }
}
