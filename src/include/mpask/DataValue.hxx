#pragma once

#include "mpask/DataSequenceElement.hxx"
#include "mpask/AliasDeclaration.hxx"

#include <string>

namespace mpask
{
  struct DataValue : public DataSequenceElement
  {
    DataValue() = default;
    ~DataValue() override = default;
    const std::string& getValue() const;
    void setValue(const std::string&);
    const std::string& getType() const override;
    void setType(const std::string&) override;

    std::ostream& print(std::ostream&) const override;

    void setContextAlias(const AliasDeclaration&);
    const AliasDeclaration& getContextAlias();

    AliasDeclaration context;
    std::string type;
    std::string value;
  };
}
