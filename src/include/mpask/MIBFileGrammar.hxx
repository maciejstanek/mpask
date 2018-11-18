#pragma once

#include "mpask/MIBFile.hxx"
#include "mpask/TypeDeclarationGrammar.hxx"
#include "mpask/ImportGrammar.hxx"
#include "mpask/ExportGrammar.hxx"
#include "mpask/SequenceDeclarationGrammar.hxx"
#include "mpask/AliasDeclarationGrammar.hxx"
#include "mpask/MacroGrammar.hxx"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <string>
#include <vector>
#include <tuple>

namespace mpask
{
  template <typename Iterator>
  struct MIBFileGrammar : boost::spirit::qi::grammar
  <
    Iterator,
    MIBFile(),
    boost::spirit::ascii::space_type
  >
  {
    MIBFileGrammar() : MIBFileGrammar::base_type(mibFile)
    {
      using boost::spirit::qi::char_;
      using boost::spirit::qi::int_;
      using boost::spirit::qi::alnum;
      using boost::spirit::qi::lit;
      using boost::spirit::qi::_val;
      using boost::spirit::qi::_1;
      using boost::spirit::qi::eps;
      using boost::spirit::qi::lexeme;

      mibFile =
        name [boost::phoenix::ref(mibFileInst.name) = _1]
        >> lit("DEFINITIONS")
        >> lit("::=")
        >> lit("BEGIN")
        >> -(
          importGrammar [boost::phoenix::ref(mibFileInst.imports) = _1] // FIXME: Only one import allowed now (and only at the top).
          )
        >> -(
          exportGrammar [boost::phoenix::ref(mibFileInst.exports) = _1] // FIXME: Only one import allowed now (and only at the top).
          )
        >> *(
          typeDeclarationGrammar [boost::phoenix::push_back(boost::phoenix::ref(mibFileInst.types), _1)]
          | sequenceDeclarationGrammar [boost::phoenix::push_back(boost::phoenix::ref(mibFileInst.sequences), _1)]
          | aliasDeclarationGrammar [boost::phoenix::push_back(boost::phoenix::ref(mibFileInst.aliases), _1)]
          | macroGrammar
          )
        >> lit("END") [_val = boost::phoenix::ref(mibFileInst)]
        ;

      name =
        +(alnum | char_('_') | char_('-'))
        ;
    }

    MIBFile mibFileInst;
    SequenceDeclarationGrammar<Iterator> sequenceDeclarationGrammar;
    TypeDeclarationGrammar<Iterator> typeDeclarationGrammar;
    ImportGrammar<Iterator> importGrammar;
    ExportGrammar<Iterator> exportGrammar;
    AliasDeclarationGrammar<Iterator> aliasDeclarationGrammar;
    MacroGrammar<Iterator> macroGrammar;
    boost::spirit::qi::rule<Iterator, MIBFile(), boost::spirit::ascii::space_type> mibFile;
    boost::spirit::qi::rule<Iterator, std::string()> name;
  };
}
