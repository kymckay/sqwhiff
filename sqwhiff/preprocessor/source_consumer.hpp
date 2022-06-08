#pragma once
#include <filesystem>
#include <istream>

#include "sqwhiff/structures/source_char.hpp"

namespace fs = std::filesystem;

namespace sqwhiff {

class SourceConsumer {
  // Reference member for polymorphism
  std::istream& source_;

  // File and position in file associated to each character
  SourceChar current_char_;

 public:
  SourceConsumer(std::istream&, fs::path = {});

  // Advance through the source by one character
  SourceChar advance();
  SourceChar peek();
  SourceChar at();
};

}  // namespace sqwhiff
