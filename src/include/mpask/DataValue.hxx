#pragma once

#include "mpask/DataSequenceElement.hxx"

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

    std::string type;
    std::string value;
  };
}
