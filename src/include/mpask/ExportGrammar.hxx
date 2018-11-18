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
  struct ExportGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    std::vector<std::string>(),
    boost::spirit::ascii::space_type
  >
  {
    ExportGrammar() : ExportGrammar::base_type(export_)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::_1;

      export_ =
        lit("EXPORTS")
        >> (name % lit(','))
        >> lit(';')
        ;

      name =
        +(alnum | char_('_') | char_('-'))
        ;
    }

    boost::spirit::qi::rule<Iterator, std::vector<std::string>(), boost::spirit::ascii::space_type> export_;
    boost::spirit::qi::rule<Iterator, std::string()> name;
  };
}
