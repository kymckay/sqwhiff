#pragma once
#include <string>

enum class TokenType {
    unknown,
    number,
    plus,
    minus,
    mul,
    div,
    end_of_file
};

struct Token {
    TokenType type;
    std::string raw;
    int line; // Line of first character in file

    Token(TokenType type = TokenType::unknown, std::string raw = "", int line = -1) : type(type), raw(raw), line(line) {}
};

// Using a typedef prevents naming clashes in global name space
typedef Token Token;