#pragma once

#include "mpask/RestrictionGrammar.hxx"
#include "mpask/IntegerValuesMapGrammar.hxx"
#include "mpask/DataType.hxx"

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
  struct DataTypeGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    DataType(),
    boost::spirit::ascii::space_type
  >
  {
    DataTypeGrammar() : DataTypeGrammar::base_type(dataType)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::_1;
      using boost::spirit::qi::eps;

      dataType =
        -sequenceOf
        >> name [boost::phoenix::ref(dataTypeInst.name) = _1]
        >> -restrictionGrammar [boost::phoenix::ref(dataTypeInst.restriction) = _1]
        >> -integerValuesMapGrammar [boost::phoenix::ref(dataTypeInst.integerValues) = _1]
        >> eps [_val = boost::phoenix::ref(dataTypeInst)]
        ;

      sequenceOf =
        lit("SEQUENCE")
        >> lit("OF") [boost::phoenix::ref(dataTypeInst.isSequence) = true]
        ;

      name =
        +(alnum | char_('_') | char_('-'))
        ;
    }

    DataType dataTypeInst;
    RestrictionGrammar<Iterator> restrictionGrammar;
    IntegerValuesMapGrammar<Iterator> integerValuesMapGrammar;
    boost::spirit::qi::rule<Iterator, DataType(), boost::spirit::ascii::space_type> dataType;
    boost::spirit::qi::rule<Iterator, std::string()> name;
    boost::spirit::qi::rule<Iterator, boost::spirit::ascii::space_type> sequenceOf;
  };
}
