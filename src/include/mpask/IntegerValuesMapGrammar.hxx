#pragma once

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
  struct IntegerValuesMapGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    std::map<std::string, int>(),
    boost::spirit::ascii::space_type
  >
  {
    IntegerValuesMapGrammar() : IntegerValuesMapGrammar::base_type(mapping)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;

      mapping.name("mapping");
      mapping =
        lit('{')
        >> mappingPair % lit(',')
        >> lit('}')
        ;

      mappingPair.name("mappingPair");
      mappingPair =
        valueName
        >> lit('(')
        >> int_
        >> lit(')')
        ;

      valueName.name("valueName");
      valueName =
        +(alnum | char_('_') | char_('-'))
        ;
    }

    boost::spirit::qi::rule<Iterator, std::map<std::string, int>(), boost::spirit::ascii::space_type> mapping;
    boost::spirit::qi::rule<Iterator, std::pair<std::string, int>(), boost::spirit::ascii::space_type> mappingPair;
    boost::spirit::qi::rule<Iterator, std::string()> valueName;
  };
}
