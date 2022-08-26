#include "sqwhiff/errors/preprocessing_error.hpp"

namespace sqwhiff {

PreprocessingError::PreprocessingError(const SourceChar& source,
                                       const std::string& message)
    : BaseError(source.line, source.column, source.file.string(), message){};

std::string PreprocessingError::type() const { return "PreprocessingError"; }

}  // namespace sqwhiff
