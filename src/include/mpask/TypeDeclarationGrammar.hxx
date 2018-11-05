#pragma once

#include "mpask/DataTypeGrammar.hxx"
#include "mpask/AddressGrammar.hxx"
#include "mpask/TypeDeclaration.hxx"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <string>
#include <vector>
#include <tuple>

namespace mpask
{
  template <typename Iterator>
  struct TypeDeclarationGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    TypeDeclaration(),
    boost::spirit::ascii::space_type
  >
  {
    TypeDeclarationGrammar() : TypeDeclarationGrammar::base_type(typeDeclaration)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::_1;
      using boost::spirit::qi::eps;
      using boost::spirit::qi::lexeme;

      typeDeclaration =
        name [boost::phoenix::ref(typeDeclarationInst.name) = _1]
        >> lit("OBJECT-TYPE")
        >> *property
        >> lit("::=")
        >> addressGrammar [
          boost::phoenix::ref(typeDeclarationInst.address) = _1,
          _val = boost::phoenix::ref(typeDeclarationInst)]
        ;
      
      property =
        syntax
        | access
        | status
        | description
        | index
        ;

      syntax =
        lit("SYNTAX")
        >> dataTypeGrammar [boost::phoenix::ref(typeDeclarationInst.syntax) = _1]
        ;

      access =
        lit("ACCESS")
        >> name [boost::phoenix::ref(typeDeclarationInst.access) = _1]
        ;

      status =
        lit("STATUS")
        >> name [boost::phoenix::ref(typeDeclarationInst.status) = _1]
        ;

      description =
        lit("DESCRIPTION")
        >> quotedText [boost::phoenix::ref(typeDeclarationInst.description) = _1]
        ;

      index =
        lit("INDEX")
        >> lit("{")
        >> (+name) [boost::phoenix::ref(typeDeclarationInst.indices) = _1]
        >> lit("}")
        ;

      name =
        +(alnum | char_('_') | char_('-'))
        ;

      quotedText =
        lexeme[
          lit('"')
          >> *(char_ - char_('"'))
          >> lit('"')]
        ;
    }

    TypeDeclaration typeDeclarationInst;
    DataTypeGrammar<Iterator> dataTypeGrammar;
    AddressGrammar<Iterator> addressGrammar;
    boost::spirit::qi::rule<Iterator, TypeDeclaration(), boost::spirit::ascii::space_type> typeDeclaration;
    boost::spirit::qi::rule<Iterator, std::string()> name;
    boost::spirit::qi::rule<Iterator, std::string()> quotedText;
    boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> property, syntax, access, status, description, index;
  };
}
