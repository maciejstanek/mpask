#include "mibparser/doStuff.hxx"

#include <iostream>

using namespace std;

namespace mibparser {

int doStuff(int thing)
{
  cerr << "Called 'mibparser::doStuff(" << thing << ")'." << endl;
  return (thing + 2) % 4;
}

}
