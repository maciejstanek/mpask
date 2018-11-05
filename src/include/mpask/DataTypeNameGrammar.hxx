#pragma once

#include "mpask/DataTypeName.hxx"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <string>

namespace mpask
{
  template <typename Iterator>
  struct DataTypeNameGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    DataTypeName(),
    boost::spirit::ascii::space_type
  >
  {
    DataTypeNameGrammar() : DataTypeNameGrammar::base_type(dataTypeName)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::_1;
      using boost::spirit::qi::eps;

      // FIXME: Only one space is currently allowed
      dataTypeName =
        (
          lit("OBJECT IDENTIFIER") [
            boost::phoenix::ref(dataTypeNameInst.name) = "OBJECT IDENTIFIER",
            boost::phoenix::ref(dataTypeNameInst.isObjectIdentifier) = true]
          | lit("OCTET STRING") [
            boost::phoenix::ref(dataTypeNameInst.name) = "OCTET STRING",
            boost::phoenix::ref(dataTypeNameInst.isOctetString) = true]
          | lit("INTEGER") [
            boost::phoenix::ref(dataTypeNameInst.name) = "INTEGER",
            boost::phoenix::ref(dataTypeNameInst.isInteger) = true]
          | lit("NULL") [
            boost::phoenix::ref(dataTypeNameInst.name) = "NULL",
            boost::phoenix::ref(dataTypeNameInst.isNull) = true]
          | nameString [
            boost::phoenix::ref(dataTypeNameInst.name) = _1]
          )
        >> eps [_val = boost::phoenix::ref(dataTypeNameInst)]
        ;

      nameString =
        +(alnum | char_('_') | char_('-'))
        ;
    }

    DataTypeName dataTypeNameInst;
    boost::spirit::qi::rule<Iterator, DataTypeName(), boost::spirit::ascii::space_type> dataTypeName;
    boost::spirit::qi::rule<Iterator, std::string()> nameString;
  };
}
