#include "sqwhiff/structures/source_string.hpp"

namespace sqwhiff {

SourceString::operator std::string() const {
  std::string result = "";
  result.reserve(chars.size());

  for (SourceChar c : chars) {
    result.push_back(c);
  }

  return result;
}

}  // namespace sqwhiff
