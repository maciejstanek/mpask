#include "mpask/Parser.hxx"

#include "mpask/Exception.hxx"
#include "mpask/CommentSkipper.hxx"
#include "mpask/MIBFileGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <streambuf>

using namespace std;

namespace mpask
{
  MIBFile
  Parser::operator()(std::istream& input) const
  {
    // TODO: Boost Spirit parser requires std::string as an input.
    //       It would be nice to have it source from std::istream.
    stringstream strippedInput;
    CommentSkipper{}.skip(input, strippedInput);
    string buffer {
      istreambuf_iterator<char>(strippedInput),
      istreambuf_iterator<char>()
    };
    string::const_iterator iter = buffer.begin();
    string::const_iterator end = buffer.end();
    MIBFileGrammar<string::const_iterator> parser;
    boost::spirit::ascii::space_type space;
    MIBFile result;
    bool status = phrase_parse(iter, end, parser, space, result);
    if (status == false) {
      throw Exception {"Parsing grammar failed."};
    }
    return result;
  }
}
