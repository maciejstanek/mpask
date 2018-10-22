#include "mpask/Parser.hxx"

#include "mpask/Exception.hxx"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <iostream>
#include <string>
#include <streambuf>
#include <complex>

using namespace std;

namespace mpask
{
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  template <typename Iterator>
  struct ObjectIdentifierParser : qi::grammar<Iterator, ascii::space_type>
  {
    ObjectIdentifierParser() : ObjectIdentifierParser::base_type(objectIdentifier)
    {
      // qi::uint_type uint_;

      // expression =
      //   term
      //   >> *(   ('+' >> term)
      //       |   ('-' >> term)
      //       )
      //   ;

      // term =
      //   factor
      //   >> *(   ('*' >> factor)
      //       |   ('/' >> factor)
      //       )
      //   ;

      // factor =
      //   uint_
      //   |   '(' >> expression >> ')'
      //   |   ('-' >> factor)
      //   |   ('+' >> factor)
      //   ;

			objectIdentifier =
				qi::lexeme[+qi::alnum]
				>> qi::lit("OBJECT")
				>> qi::lit("IDENTIFIER")
        >> qi::lit("::=")
        >> qi::lit("{")
        >> qi::lexeme[+qi::alnum]
        >> *qi::lexeme[+qi::alnum >> '(' >> qi::int_ >> ')']
        >> qi::int_
        >> qi::lit("}")
				;
    }

    // qi::rule<Iterator, ascii::space_type> expression, term, factor;
    qi::rule<Iterator, ascii::space_type> objectIdentifier;
  };

  void
  Parser::parse(std::istream& input) const
  {
    // FIXME: Boost Spirit parser requires std::string as an input.
    //        It would be nice to have it source from std::istream.
    string buffer {istreambuf_iterator<char>(input), istreambuf_iterator<char>()};

		// SANDBOX
    boost::spirit::ascii::space_type space;
    ObjectIdentifierParser<string::const_iterator> parser;
		string::const_iterator iter = buffer.begin();
    string::const_iterator end = buffer.end();
    bool status = phrase_parse(iter, end, parser, space);
    if (status == false) {
      throw Exception {"Parsing grammar failed."};
    }
  }
}
