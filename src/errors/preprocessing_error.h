#pragma once
#include <string>

#include "./base_error.h"

namespace sqwhiff {

class PreprocessingError : public BaseError {
 public:
  using BaseError::BaseError;
  using BaseError::pretty;

  std::string type() const override;
};

}  // namespace sqwhiff
