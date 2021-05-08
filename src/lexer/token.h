#pragma once
#include "src/lexer/token_types.h"
#include <string>

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