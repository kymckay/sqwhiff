#pragma once
#include <string>

namespace sqwhiff {

// Character with an associated file position
struct SourceChar {
  char character;
  int line;
  int column;
  std::string file;

  // Convenience operators for manipulating the underlying char
  operator char();
  bool operator==(char) const;
  void operator=(char);
};

}  // namespace sqwhiff
