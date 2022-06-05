#include "./syntax_error.h"

const std::string SyntaxError::id = "SyntaxError";

std::string SyntaxError::pretty() const {
  return file + "\n\n\t" + std::to_string(line) + ":" + std::to_string(col) +
         " " + id + " - " + what();
}
