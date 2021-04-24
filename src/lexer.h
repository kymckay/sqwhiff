#pragma once
#include "token.h"
#include <string>
#include <array>
#include <fstream>

#define NUM_DELIMITERS 28

class Lexer
{
    const static std::array<char, NUM_DELIMITERS> delimiters_;

    std::ifstream file_;
    char current_char_;
    int line_;

    void advance();
    void skipWhitespace();
    std::string number();
    bool isDelimiter(const char);

public:
    Lexer(std::string);
    ~Lexer();
    Token nextToken();
};