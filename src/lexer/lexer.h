#pragma once
#include "src/lexer/token.h"
#include <string>
#include <array>
#include <istream>

class Lexer
{
    // Reference member, stream has no copy constructor and lexer doesn't care what kind of stream it is (file or string)
    std::istream &stream_;

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
    Token string();

public:
    Lexer(std::istream&);
    Token nextToken();
};