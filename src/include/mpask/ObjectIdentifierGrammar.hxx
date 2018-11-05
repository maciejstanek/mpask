#pragma once

#include "mpask/ObjectIdentifier.hxx"
#include "mpask/AddressGrammar.hxx"

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
  struct ObjectIdentifierGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    ObjectIdentifier(),
    boost::spirit::ascii::space_type
  >
  {
    ObjectIdentifierGrammar() : ObjectIdentifierGrammar::base_type(objectIdentifier)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_1;
      using boost::spirit::qi::_val;

      objectIdentifier =
        name [boost::phoenix::ref(objectIdentifierInst.identifier) = _1]
        >> lit("OBJECT")
        >> lit("IDENTIFIER")
        >> lit("::=")
        >> addressGrammar [boost::phoenix::ref(objectIdentifierInst.address) = _1, _val = boost::phoenix::ref(objectIdentifierInst)]
        ;

      name =
        +(alnum | char_('_') | char_('-'))
        ;
    }

    ObjectIdentifier objectIdentifierInst;
    AddressGrammar<Iterator> addressGrammar;
    boost::spirit::qi::rule<Iterator, ObjectIdentifier(), boost::spirit::ascii::space_type> objectIdentifier;
    boost::spirit::qi::rule<Iterator, std::string()> name;
  };
}
