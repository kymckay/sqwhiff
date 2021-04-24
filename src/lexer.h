#pragma once
#include "token.h"
#include <string>
#include <array>
#include <fstream>

#define NUM_DELIMITERS 28

class Lexer
{
    const static std::array<char, NUM_DELIMITERS> delimiters_;

    // This is a reference member because ifstream has no copy constructor
    // which would prevent Lexer being used as a parameter elsewhere
    std::ifstream &file_;

    char current_char_;
    // Current line lexer has reached in the text
    // Used to give a position to errors
    int line_ = 0;

    void error();
    void advance();
    void skipWhitespace();
    std::string number();
    bool isDelimiter(const char);

public:
    Lexer(std::ifstream&);
    ~Lexer();
    Token nextToken();
};