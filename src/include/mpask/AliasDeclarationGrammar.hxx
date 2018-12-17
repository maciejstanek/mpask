#pragma once

#include "mpask/DataTypeGrammar.hxx"
#include "mpask/AliasDeclaration.hxx"

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
  struct AliasDeclarationGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    AliasDeclaration(),
    boost::spirit::ascii::space_type
  >
  {
    AliasDeclarationGrammar() : AliasDeclarationGrammar::base_type(aliasDeclaration)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::_1;
      using boost::spirit::qi::eps;
      using boost::spirit::qi::lexeme;

      aliasDeclaration =
        name [boost::phoenix::ref(aliasDeclarationInst.name) = _1]
        >> lit("::=")
        >> -(
          lit('[')
          >> -visibility
          >> int_ [boost::phoenix::ref(aliasDeclarationInst.typeIdentifier) = _1]
          >> lit(']')
          )
        >> -explicity
        >> dataTypeGrammar [
          boost::phoenix::ref(aliasDeclarationInst.dataType) = _1,
          _val = boost::phoenix::ref(aliasDeclarationInst)]
        ;

      explicity =
        lit("IMPLICIT") [boost::phoenix::ref(aliasDeclarationInst.isImplicit) = true]
        | lit("EXPLICIT") [boost::phoenix::ref(aliasDeclarationInst.isExplicit) = true]
        ;

      visibility =
        lit("UNIVERSAL") [boost::phoenix::ref(aliasDeclarationInst.visibility) = std::string("UNIVERSAL")]
        | lit("APPLICATION") [boost::phoenix::ref(aliasDeclarationInst.visibility) = std::string("APPLICATION")]
        | lit("CONTEXT-SPECIFIC") [boost::phoenix::ref(aliasDeclarationInst.visibility) = std::string("CONTEXT-SPECIFIC")]
        | lit("PRIVATE") [boost::phoenix::ref(aliasDeclarationInst.visibility) = std::string("PRIVATE")]
        ;

      name =
        +(alnum | char_('_') | char_('-'))
        ;
    }

    AliasDeclaration aliasDeclarationInst;
    DataTypeGrammar<Iterator> dataTypeGrammar;
    boost::spirit::qi::rule<Iterator, AliasDeclaration(), boost::spirit::ascii::space_type> aliasDeclaration;
    boost::spirit::qi::rule<Iterator, std::string()> name;
    boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> explicity, visibility;
  };
}
