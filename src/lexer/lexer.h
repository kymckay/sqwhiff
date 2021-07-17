#pragma once
#include "./token.h"
#include "./lexical_error.h"
#include "../preprocessor/preprocessor.h"
#include <deque>
#include <string>
#include <istream>

class Lexer
{
    // Reference member, no need to copy the supplied preprocessor
    Preprocessor &preproc_;

    // Use buffer to simplify lookahead logic
    std::deque<Token> peek_buffer_;

    PosChar current_char_;

    // First time used need to preload the initial character
    bool first_use_ = true;
    inline void init()
    {
        if (first_use_)
        {
            first_use_ = false;
            advance();
        }
    }

    void error(Token, std::string);
    void advance();
    void skipWhitespace();
    Token _id();
    Token number();
    Token string();
    Token makeToken(TokenType, std::string);
    Token nextToken();

public:
    Lexer(Preprocessor&);
    Token get();
    Token peek(int = 1);
};