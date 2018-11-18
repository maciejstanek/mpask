#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <string>

namespace mpask
{
  template <typename Iterator>
  struct MacroGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    std::string(),
    boost::spirit::ascii::space_type
  >
  {
    MacroGrammar() : MacroGrammar::base_type(macro)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::_1;
      using boost::spirit::qi::lexeme;

      macro =
        name [_val = _1]
        >> lit("MACRO")
        >> lit("::=")
        >> lexeme [
          lit("BEGIN")
          >> *(char_ - lit("END"))
          >> lit("END")
          ]
        ;

      name =
        +(alnum | char_('_') | char_('-'))
        ;
    }

    boost::spirit::qi::rule<Iterator, std::string(), boost::spirit::ascii::space_type> macro;
    boost::spirit::qi::rule<Iterator, std::string()> name;
  };
}
