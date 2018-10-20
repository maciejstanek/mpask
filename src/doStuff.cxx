#include "mibparser/doStuff.hxx"

#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;
using namespace boost;

namespace mibparser {

int doStuff(int thing)
{
  cerr << "Called 'mibparser::doStuff(" << lexical_cast<std::string>(thing) << ")'." << endl;
  return (thing + 2) % 4;
}

}
