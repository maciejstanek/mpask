#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <string>
#include <vector>

namespace mpask
{
  template <typename Iterator>
  struct ImportGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    std::vector<std::pair<std::vector<std::string>, std::string>>(),
    boost::spirit::ascii::space_type
  >
  {
    ImportGrammar() : ImportGrammar::base_type(import)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::_1;

      import =
        lit("IMPORTS")
        >> +source
        >> lit(';')
        ;

      source =
        (name % lit(','))
        >> lit("FROM")
        >> name
        ;

      name =
        +(alnum | char_('_') | char_('-'))
        ;
    }

    boost::spirit::qi::rule<Iterator, std::vector<std::pair<std::vector<std::string>, std::string>>(), boost::spirit::ascii::space_type> import;
    boost::spirit::qi::rule<Iterator, std::pair<std::vector<std::string>, std::string>(), boost::spirit::ascii::space_type> source;
    boost::spirit::qi::rule<Iterator, std::string()> name;
  };
}
