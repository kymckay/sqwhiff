#pragma once
#include <stdexcept>
#include <string>

class LexicalError : public std::runtime_error {
 public:
  static const std::string id;

  const int line;
  const int col;
  const std::string file;

  LexicalError(int line, int col, std::string file, std::string msg)
      : std::runtime_error(msg), line(line), col(col), file(file) {}

  std::string pretty() const;
};
