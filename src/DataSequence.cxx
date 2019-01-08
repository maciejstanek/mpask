#include "mpask/DataSequence.hxx"

#include <sstream>

using namespace std;

namespace mpask
{
  DataSequence::Iterator
  DataSequence::begin()
  {
    return sequence.begin();
  }

  DataSequence::Iterator
  DataSequence::end()
  {
    return sequence.end();
  }

  void
  DataSequence::append(const shared_ptr<DataSequenceElement>& elem)
  {
    sequence.push_back(elem);
  }

  const string&
  DataSequence::getType() const
  {
    return type;
  }

  void
  DataSequence::setType(const string& x)
  {
    type = x;
  }

  void
  DataSequence::setContextSequence(const SequenceDeclaration& newContext)
  {
    context = newContext;
  }

  const SequenceDeclaration&
  DataSequence::getContextSequence()
  {
    return context;
  }

  ostream&
  DataSequence::print(ostream& s) const
  {
    s << "SEQUENCE\n";
    for (const auto& element : sequence) {
      stringstream ss;
      ss << *element;
      string ssline;
      while (std::getline(ss, ssline)) {
        if (ssline != "") {
          s << "    " << ssline << "\n";
        }
      }
    }
    return s;
  }
}
