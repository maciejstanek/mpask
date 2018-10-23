#include "mpask/Parser.hxx"

#include "mpask/Exception.hxx"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_no_skip.hpp>
#include <iostream>
#include <string>
#include <streambuf>

using namespace std;

namespace mpask
{
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  template <typename Iterator>
  struct DefinitionParser : qi::grammar<Iterator, ascii::space_type>
  {
    DefinitionParser() : DefinitionParser::base_type(definition)
    {
      definition =
        objectIdentifierDefinition
        | objectTypeDefinition
        | dataTypeDefinition
        ;

      objectIdentifierDefinition =
        identifier
        >> qi::lit("OBJECT")
        >> qi::lit("IDENTIFIER")
        >> qi::lit("::=")
        >> definitionAddress
        ;

      definitionAddress =
        qi::lit("{")
        >> identifier
        >> *(identifier >> '(' >> qi::int_ >> ')') // TODO: Probably should somehow disable spaces.
        >> qi::int_
        >> qi::lit("}")
        ;

      identifier =
        qi::lexeme[+(qi::alnum | qi::char_('_') | qi::char_('-'))]
        ;

      longText =
        qi::lexeme[
          qi::char_('"')
          >> *(qi::char_ - qi::char_('"'))
          >> qi::char_('"')
          ]
        ;

      objectTypeDefinition =
        identifier
        >> qi::lit("OBJECT-TYPE")
        >> *objectTypeProperty
        >> qi::lit("::=")
        >> definitionAddress
        ;

      objectTypeProperty =
        syntaxProperty
        | accessProperty
        | statusProperty
        | descriptionProperty
        ;

      syntaxProperty =
        qi::lit("SYNTAX")
        >> identifier
        ;

      accessProperty =
        qi::lit("ACCESS")
        >> identifier
        ;

      statusProperty =
        qi::lit("STATUS")
        >> identifier
        ;

      descriptionProperty =
        qi::lit("DESCRIPTION")
        >> longText
        ;

      restriction = // TODO: Should not skip spaces.
        qi::char_('(')
        >> (
          rangeRestriction
          | sizeRestriction
          )
        >> qi::char_(')')
        ;

      rangeRestriction = // TODO: Should not skip spaces.
        qi::int_
        >> qi::lit("..")
        >> qi::int_
        ;

      sizeRestriction = // TODO: Should not skip spaces.
        qi::lit("SIZE")
        >> qi::int_
        ;

      simpleDataType =
        (qi::lit("OCTET") >> qi::lit("STRING"))
        | qi::lit("INTEGER")
        | (qi::lit("OBJECT") >> qi::lit("IDENTIFIER"))
        | qi::lit("NULL")
        ;

      dataTypeDefinition =
        identifier
        >> qi::lit("::=")
        >> -(qi::char_('[') >> visibility >> qi::int_ >> qi::char_(']'))
        >> -explicity
        >> simpleDataType
        >> -sizeRestriction
        ;

      visibility =
        qi::lit("UNIVERSAL")
        | qi::lit("APPLICATION")
        | qi::lit("CONTEXT-SPECIFIC")
        | qi::lit("PRIVATE")
        ;

      explicity =
        qi::lit("IMPLICIT")
        | qi::lit("EXPLICIT")
        ;

      // TODO: Implement 'SEQUENCE' oraz 'SEQUENCE OF'.

      // NOTE: Useful debugging function
      // qi::debug(definition);
    }

    qi::rule<Iterator, ascii::space_type> definition, syntaxProperty,
      accessProperty, statusProperty, descriptionProperty,
      objectTypeProperty, definitionAddress, objectIdentifierDefinition,
      objectTypeDefinition, identifier, longText, dataTypeDefinition,
      simpleDataType, restriction, sizeRestriction, rangeRestriction,
      visibility, explicity;
  };

  void
  Parser::parse(std::istream& input) const
  {
    // FIXME: Boost Spirit parser requires std::string as an input.
    //        It would be nice to have it source from std::istream.
    string buffer {istreambuf_iterator<char>(input), istreambuf_iterator<char>()};

    // SANDBOX
    boost::spirit::ascii::space_type space;
    DefinitionParser<string::const_iterator> parser;
    string::const_iterator iter = buffer.begin();
    string::const_iterator end = buffer.end();
    bool status = phrase_parse(iter, end, parser, space);
    if (status == false) {
      throw Exception {"Parsing grammar failed."};
    }
  }
}
