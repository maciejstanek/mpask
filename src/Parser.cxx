#include "mpask/Parser.hxx"

#include "mpask/Exception.hxx"
#include "mpask/CommentSkipper.hxx"

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
    DefinitionParser() : DefinitionParser::base_type(entryPoint)
    {
      entryPoint =
        +definition
        ;

      definition =
        objectIdentifierDefinition
        | objectTypeDefinition
        | dataTypeDefinition
        | importDefinition
        | nestingDefinition
        | sequenceDefinition
        ;

      sequenceDefinition =
        identifier
        >> qi::lit("::=")
        >> qi::lit("SEQUENCE")
        >> qi::char_('{')
        >> sequenceElement % qi::char_(',')
        >> qi::char_('}')
        ;

      sequenceElement =
        identifier
        >> (
          objectIdentifierLabel
          | simpleDataType
          | identifier
          )
        >> -restriction
        ;

      importDefinition =
        qi::lit("IMPORTS")
        >> +importList
        >> qi::lit(";")
        ;

      importList =
        identifier % qi::lit(",")
        >> qi::lit("FROM")
        >> identifier
        ;

      nestingDefinition =
        identifier
        >> qi::lit("DEFINITIONS")
        >> qi::lit("::=")
        >> qi::lit("BEGIN")
        >> *definition
        >> qi::lit("END")
        ;

      objectIdentifierDefinition =
        identifier
        >> objectIdentifierLabel
        >> qi::lit("::=")
        >> definitionAddress
        ;

      objectIdentifierLabel =
        qi::lit("OBJECT")
        >> qi::lit("IDENTIFIER")
        ;

      definitionAddress.name("definitionAddress");
      definitionAddress =
        qi::lit("{")
        >> identifier
        >> *namedIndex // TODO: Probably should somehow disable spaces.
        >> qi::int_
        >> qi::lit("}")
        ;

      identifier.name("identifier");
      identifier =
        qi::lexeme[+(qi::alnum | qi::char_('_') | qi::char_('-'))]
        ;

      longText.name("longText");
      longText =
        qi::lexeme[
          qi::char_('"')
          >> *(qi::char_ - qi::char_('"'))
          >> qi::char_('"')
          ]
        ;

      objectTypeDefinition.name("objectTypeDefinition");
      objectTypeDefinition =
        identifier
        >> qi::lit("OBJECT-TYPE")
        >> *objectTypeProperty
        >> qi::lit("::=")
        >> definitionAddress
        ;

      objectTypeProperty.name("objectTypeProperty");
      objectTypeProperty =
        syntaxProperty
        | accessProperty
        | statusProperty
        | descriptionProperty
        | indexProperty
        ;

      indexProperty.name("indexProperty");
      indexProperty =
        qi::lit("INDEX")
        >> qi::char_('{')
        >> identifier % ','
        >> qi::char_('}')
        ;

      syntaxProperty.name("syntaxProperty");
      syntaxProperty =
        qi::lit("SYNTAX")
        >> ((sequenceOf | simpleDataType | identifier) >> -restriction)
        ;

      sequenceOf.name("sequenceOf");
      sequenceOf =
        qi::lit("SEQUENCE")
        >> qi::lit("OF")
        >> identifier
        ;

      restriction.name("restriction");
      restriction =
        qi::char_('(')
        >> (
          rangeRestriction
          | sizeRestriction
          )
        >> qi::char_(')')
        ;

      rangeRestriction.name("rangeRestriction");
      rangeRestriction =
        qi::int_
        >> qi::lit("..")
        >> qi::int_
        ;

      sizeRestriction.name("sizeRestriction");
      sizeRestriction =
        qi::lit("SIZE")
        >> qi::char_('(')
        >> (
          rangeRestriction
          | qi::int_
          )
        >> qi::char_(')')
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

      simpleDataType =
        (qi::lit("OCTET") >> qi::lit("STRING"))
        | integer
        | objectIdentifierLabel
        | qi::lit("NULL")
        ;

      integer =
        qi::lit("INTEGER")
        >> -(
          qi::char_('{')
          >> namedIndex % qi::char_(',')
          >> qi::char_('}')
          )
        ;

      namedIndex =
        identifier
        >> '('
        >> qi::int_
        >> ')' // TODO: Probably should somehow disable spaces.
        ;

      dataTypeDefinition =
        identifier
        >> qi::lit("::=")
        >> -(qi::char_('[') >> visibility >> qi::int_ >> qi::char_(']'))
        >> -explicity
        >> simpleDataType
        >> -restriction
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

      // NOTE: Useful debugging function
      /* qi::debug(entryPoint); */
      /* qi::debug(nestingDefinition); */
      /* qi::debug(definition); */
      /* qi::debug(restriction); */
      /* qi::debug(objectTypeProperty); */
      /* qi::debug(syntaxProperty); */
      /* qi::debug(sizeRestriction); */
      /* qi::debug(rangeRestriction); */
      /* qi::debug(sequenceOf); */
    }

    qi::rule<Iterator, ascii::space_type> definition, syntaxProperty,
      accessProperty, statusProperty, descriptionProperty,
      objectTypeProperty, definitionAddress, objectIdentifierDefinition,
      objectTypeDefinition, identifier, longText, dataTypeDefinition,
      simpleDataType, restriction, visibility, explicity, entryPoint,
      nestingDefinition, importList, importDefinition, sizeRestriction,
      rangeRestriction, objectIdentifierLabel, sequenceOf, indexProperty,
      sequenceDefinition, sequenceElement, integer, namedIndex;
  };

  void
  Parser::parse(std::istream& input) const
  {
    // FIXME: Boost Spirit parser requires std::string as an input.
    //        It would be nice to have it source from std::istream.
    stringstream strippedInput;
    CommentSkipper{}.skip(input, strippedInput);
    string buffer {
      istreambuf_iterator<char>(strippedInput),
      istreambuf_iterator<char>()
    };

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
