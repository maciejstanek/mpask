#pragma once

#include <exception>
#include <string>

namespace mpask
{
  class Exception : public std::exception
  {
  public:
    explicit Exception
      ( const std::string& what
      )
      : std::exception {}
      , what_ {what}
    {
    }
    virtual ~Exception()
    {
    }
    const char* what() const noexcept override
    {
      return what_.c_str();
    }
  private:
    std::string what_;
  };
}
