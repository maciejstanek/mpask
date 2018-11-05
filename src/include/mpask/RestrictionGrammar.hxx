#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include "mpask/Restriction.hxx"

namespace mpask
{
  template <typename Iterator>
  struct RestrictionGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    Restriction(),
    boost::spirit::ascii::space_type
  >
  {
    RestrictionGrammar() : RestrictionGrammar::base_type(restriction)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::lit;
      using boost::spirit::ascii::space_type;
      using boost::spirit::qi::_1;
      using boost::spirit::qi::lexeme;
      using boost::spirit::qi::_val;
      using boost::phoenix::ref;

      restriction.name("restriction");
      restriction =
        char_('(')
        >> (
          size
          | numbers
          )
        >> char_(')') [_val = ref(restrictionInst)]
        ;

      numbers.name("numbers");
      numbers =
        int_ [ref(restrictionInst.left) = _1, ref(restrictionInst.right) = _1]
        >> -(
          lit("..") [ref(restrictionInst.range) = true]
          >> int_ [ref(restrictionInst.right) = _1]
          )
        ;

      size.name("size");
      size =
        lit("SIZE") [ref(restrictionInst.size) = true]
        >> char_('(')
        >> numbers
        >> char_(')')
        ;
    }

    Restriction restrictionInst;
    boost::spirit::qi::rule<Iterator, Restriction(), boost::spirit::ascii::space_type> restriction;
    boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> numbers;
    boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> size;
  };
}
