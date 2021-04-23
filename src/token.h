#pragma once
#include <string>

struct Token {
    std::string content;
    int line; // Line and column of first character in overall text

    Token() {}

    Token(std::string raw, int line)
    {
        content = raw;
        this->line = line;
    }
};

// Using a typedef prevents naming clashes in global name space
typedef Token Token;