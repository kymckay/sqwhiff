#include "./preprocessing_error.h"

const std::string PreprocessingError::id = "PreprocessingError";

std::string PreprocessingError::pretty() const {
  return file + "\n\n\t" + std::to_string(line) + ":" + std::to_string(col) +
         " " + id + " - " + what();
}
