#pragma once
#include <stdexcept>
#include <string>

class SemanticError : public std::runtime_error {
 public:
  SemanticError(int line, int col, std::string msg)
      : std::runtime_error(std::to_string(line) + ":" + std::to_string(col) +
                           " SemanticError - " + msg) {}
};
