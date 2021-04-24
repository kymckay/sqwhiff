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
    TokenType type = TokenType::unknown;
    std::string raw;
    int line; // Line and column of first character in overall text

    Token() {}

    Token(std::string raw, int line)
    {
        this->raw = raw;
        this->line = line;
    }

    Token(TokenType type, std::string raw, int line)
    {
        this->type = type;
        this->raw = raw;
        this->line = line;
    }
};

// Using a typedef prevents naming clashes in global name space
typedef Token Token;