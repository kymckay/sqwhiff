#pragma once
#include "src/lexer/token.h"
#include "src/lexer/lexical_error.h"
#include <string>
#include <istream>

class Lexer
{
    // Reference member, stream has no copy constructor and lexer doesn't care what kind of stream it is (file or string)
    std::istream &stream_;

    char current_char_;
    // Current position lexer has reached in the text
    // Used to give a position to errors
    int lineno_ = 1;
    int column_ = 1;

    void error(Token, std::string);
    char peek();
    void advance();
    void skipWhitespace();
    void skipComment();
    Token _id();
    Token number();
    Token string();
    Token makeToken(TokenType, std::string);

public:
    Lexer(std::istream&);
    Token nextToken();
};