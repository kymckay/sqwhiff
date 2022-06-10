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

void SourceString::append(const SourceString& tail) {
  chars.reserve(tail.chars.size());
  chars.insert(chars.end(), tail.chars.begin(), tail.chars.end());
}

}  // namespace sqwhiff
