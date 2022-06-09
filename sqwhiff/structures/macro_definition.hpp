#pragma once
#include <vector>

#include "sqwhiff/structures/source_string.hpp"

namespace sqwhiff {

struct MacroDefinition {
  // The macro's body, split by paramter occurances to replace when used
  std::vector<SourceString> body;
  // Indexes of arguments to insert at each break in the body
  std::vector<int> insertions;
};

}  // namespace sqwhiff
