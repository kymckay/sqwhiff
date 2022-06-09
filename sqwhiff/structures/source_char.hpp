#pragma once
#include <filesystem>
#include <string>

namespace sqwhiff {

// Character with an associated file position
struct SourceChar {
  char character;
  int line;
  int column;
  std::filesystem::path file;

  // Convenience operators for manipulating the underlying char
  operator char();
  operator char() const;
  bool operator==(char) const;
  void operator=(char);
};

}  // namespace sqwhiff
