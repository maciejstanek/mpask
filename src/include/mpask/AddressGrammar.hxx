#pragma once

#include "mpask/Address.hxx"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <string>
#include <vector>
#include <tuple>

namespace mpask
{
  template <typename Iterator>
  struct AddressGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    Address(),
    boost::spirit::ascii::space_type
  >
  {
    AddressGrammar() : AddressGrammar::base_type(address)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::_1;
      /* using phx = boost::spirit::phoenix; */

      address =
        lit('{')
        >> name [boost::phoenix::ref(addressInst.label) = _1]
        >> (*intermediateNode) [boost::phoenix::ref(addressInst.intermediateNodes) = _1]
        >> int_ [boost::phoenix::ref(addressInst.value) = _1]
        >> lit('}') [_val = boost::phoenix::ref(addressInst)]
        ;

      intermediateNode =
        name
        >> lit('(')
        >> int_
        >> lit(')')
        ;

      name =
        +(alnum | char_('_') | char_('-'))
        ;
    }

    Address addressInst;
    boost::spirit::qi::rule<Iterator, Address(), boost::spirit::ascii::space_type> address;
    boost::spirit::qi::rule<Iterator, std::pair<std::string, int>(), boost::spirit::ascii::space_type> intermediateNode;
    boost::spirit::qi::rule<Iterator, std::string()> name;
  };
}
