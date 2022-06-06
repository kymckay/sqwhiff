#pragma once
#include <string>

#include "sqwhiff/tokens/token_types.hpp"

struct Token {
  static const int npos = -1;

  TokenType type = TokenType::unknown;
  std::string raw = "";
  // Position of token's first character in the file
  int line = npos;
  int column = npos;
  std::string file = "";
};

// Using a typedef prevents naming clashes in global name space
typedef Token Token;
