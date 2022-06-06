#pragma once
#include <stdexcept>
#include <string>

namespace sqwhiff {

class BaseError : public std::runtime_error {
 public:
  const int line;
  const int col;
  const std::string file;

  BaseError(int line, int col, std::string file, std::string msg)
      : std::runtime_error(msg), line(line), col(col), file(file) {}

  virtual ~BaseError() = default;

  // Unique (in context of file error belongs to) identifer for storage
  std::string uid() const;
  virtual std::string type() const = 0;
};

}  // namespace sqwhiff
