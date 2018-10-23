#include "mpask/CommentSkipper.hxx"

#include "mpask/Exception.hxx"

#include <iostream>
#include <string>
#include <streambuf>

using namespace std;

namespace mpask
{
  void
  CommentSkipper::skip(istream& input, ostream& output) const
  {
    char character;
    bool insideComment {false};
    while (input >> noskipws >> character) {
      if (insideComment) {
        if (character == '\n') {
          insideComment = false;
          output << character;
        }
      }
      else {
        if (character == '-') {
          char secondCharacter;
          if (input >> noskipws >> secondCharacter) {
            if (secondCharacter == '-') {
              insideComment = true;
            }
            else {
              output << character << secondCharacter;
            }
          }
          else {
            output << character;
          }
        }
        else {
          output << character;
        }
      }
    }
  }
}
