#include "./semantic_error.h"

const std::string SemanticError::id = "SemanticError";

std::string SemanticError::pretty() const {
  return file + "\n\n\t" + std::to_string(line) + ":" + std::to_string(col) +
         " " + id + " - " + what();
}
