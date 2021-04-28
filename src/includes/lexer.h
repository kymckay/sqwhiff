#pragma once
#include "token.h"
#include <string>
#include <array>
#include <fstream>

class Lexer
{
    // Reference member, ifstream has no copy constructor but we want to supply one to the lexer
    std::ifstream &file_;

    char current_char_;
    // Current line lexer has reached in the text
    // Used to give a position to errors
    int line_ = 0;

    void error();
    char peek();
    void advance();
    void skipWhitespace();
    void skipComment();
    Token _id();
    Token number();

public:
    Lexer(std::ifstream&);
    ~Lexer();
    Token nextToken();
};