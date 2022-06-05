#include "./base_error.h"

namespace sqwhiff {

// UID when sorted will group by file then order by line and column
std::string BaseError::uid() const {
  return file + "," + std::to_string(line) + '.' + std::to_string(col) + '.' +
         type() + ',' + what();
}

}  // namespace sqwhiff
