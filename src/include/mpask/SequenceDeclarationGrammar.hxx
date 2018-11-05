#pragma once

#include "mpask/SequenceDeclaration.hxx"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <string>
#include <map>

namespace mpask
{
  template <typename Iterator>
  struct SequenceDeclarationGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    SequenceDeclaration(),
    boost::spirit::ascii::space_type
  >
  {
    SequenceDeclarationGrammar() : SequenceDeclarationGrammar::base_type(sequenceDeclaration)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::_1;

      sequenceDeclaration =
        name [boost::phoenix::ref(sequenceDeclarationInst.name) = _1]
        >> lit("::=")
        >> lit("SEQUENCE")
        >> lit('{')
        >> sequenceElements [boost::phoenix::ref(sequenceDeclarationInst.sequence) = _1]
        >> lit('}') [_val = boost::phoenix::ref(sequenceDeclarationInst)]
        ;

      sequenceElements =
        sequenceElement % lit(',')
        ;

      sequenceElement =
        name
        >> valueName
        ;

      name =
        +(alnum | char_('_') | char_('-'))
        ;

      valueName =
        lit("OBJECT IDENTIFIER") [_val = std::string("OBJECT IDENTIFIER")]
        | name [_val = _1]
        ;
    }

    SequenceDeclaration sequenceDeclarationInst;
    boost::spirit::qi::rule<Iterator, SequenceDeclaration(), boost::spirit::ascii::space_type> sequenceDeclaration;
    boost::spirit::qi::rule<Iterator, std::pair<std::string, std::string>(), boost::spirit::ascii::space_type> sequenceElement;
    boost::spirit::qi::rule<Iterator, std::map<std::string, std::string>(), boost::spirit::ascii::space_type> sequenceElements;
    boost::spirit::qi::rule<Iterator, std::string()> name, valueName;
  };
}
