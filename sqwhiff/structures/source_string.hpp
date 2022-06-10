#pragma once
#include <string>
#include <vector>

#include "sqwhiff/structures/source_char.hpp"

namespace sqwhiff {

// Sequence of characters with associated file positions
class SourceString {
 public:
  std::vector<SourceChar> chars = {};
  operator std::string() const;

  void append(const SourceString&);
};

}  // namespace sqwhiff
