#include "./lexical_error.h"

const std::string LexicalError::id = "LexicalError";

std::string LexicalError::pretty() const {
  return file + "\n\n\t" + std::to_string(line) + ":" + std::to_string(col) +
         " " + id + " - " + what();
}
