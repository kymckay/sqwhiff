#pragma once
#include "src/tokens/token_type.h"
#include <string>

struct Token {
    TokenType type;
    std::string raw;
    int line; // Line of first character in file

    Token(TokenType type = TokenType::unknown, std::string raw = "", int line = -1) : type(type), raw(raw), line(line) {}
};

// Using a typedef prevents naming clashes in global name space
typedef Token Token;