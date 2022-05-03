#pragma once
#include <string>

#include "./token_types.h"

struct Token {
  static const int npos = -1;

  TokenType type = TokenType::unknown;
  std::string raw = "";
  // Position of token's first character in the file
  int line = npos;
  int column = npos;
};

// Using a typedef prevents naming clashes in global name space
typedef Token Token;
