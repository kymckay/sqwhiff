#pragma once
#include <string>

#include "sqwhiff/errors/base_error.hpp"

namespace sqwhiff {

class LexicalError : public BaseError {
 public:
  using BaseError::BaseError;
  using BaseError::uid;

  std::string type() const override;
};

}  // namespace sqwhiff
