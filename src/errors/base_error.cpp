#include "./base_error.h"

namespace sqwhiff {

std::string BaseError::pretty() const {
  return file + "\n\n\t" + std::to_string(line) + ":" + std::to_string(col) +
         " " + type() + " - " + what();
}

}  // namespace sqwhiff
