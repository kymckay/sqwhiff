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
    char *delimiter_ = nullptr;

    // Current line lexer has reached in the text (1-indexed)
    // Used to give tokens a position for later parsing errors
    int line_ = 1;

    Token nextRawToken();
    bool isDelimiter(const char);

public:
    Lexer(std::string);
    ~Lexer();
    Token nextToken();
};