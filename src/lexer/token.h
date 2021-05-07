#pragma once
#include "src/lexer/token_types.h"
#include <string>

struct Token {
    static const int npos = -1;

    TokenType type;
    std::string raw;
    // Position of token's first character in the file
    int line;
    int column;

    Token(TokenType type = TokenType::unknown, std::string raw = "", int line = npos, int column = npos)
        : type(type), raw(raw), line(line), column(column) {}
};

// Using a typedef prevents naming clashes in global name space
typedef Token Token;