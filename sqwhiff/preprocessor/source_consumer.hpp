#pragma once
#include <filesystem>
#include <istream>

#include "sqwhiff/interfaces/abstract_consumer.hpp"
#include "sqwhiff/structures/source_char.hpp"

namespace fs = std::filesystem;

namespace sqwhiff {

class SourceConsumer : public AbstractConsumer<SourceChar> {
  // Reference member for polymorphism
  std::istream& source_;

  // File and position in file associated to each character
  SourceChar current_char_;

 public:
  SourceConsumer(std::istream&, fs::path = {});

  // Advance through the source by one character
  SourceChar advance() override;
  SourceChar peek() override;
  SourceChar at() override;
};

}  // namespace sqwhiff
