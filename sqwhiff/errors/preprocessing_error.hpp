#pragma once
#include <string>

#include "sqwhiff/errors/base_error.hpp"
#include "sqwhiff/structures/source_char.hpp"

namespace sqwhiff {

class PreprocessingError : public BaseError {
 public:
  using BaseError::uid;

  PreprocessingError(const SourceChar&, const std::string&);

  std::string type() const override;
};

}  // namespace sqwhiff
