#pragma once

#include "mpask/DataSequenceElement.hxx"
#include "mpask/SequenceDeclaration.hxx"

#include <memory>
#include <vector>

namespace mpask
{
  struct DataSequence : public DataSequenceElement
  {
    using Iterator = std::vector<std::shared_ptr<DataSequenceElement>>::iterator;
    DataSequence() = default;
    ~DataSequence() override = default;
    void append(const std::shared_ptr<DataSequenceElement>&);
    Iterator begin();
    Iterator end();
    const std::string& getType() const override;
    void setType(const std::string&) override;
    void setContextSequence(const SequenceDeclaration&);
    const SequenceDeclaration& getContextSequence();
    std::ostream& print(std::ostream&) const override;

    SequenceDeclaration context;
    std::string type;
    std::vector<std::shared_ptr<DataSequenceElement>> sequence;
  };
}
