#pragma once
#include "src/preprocessor/preprocessor.h"
#include "src/lexer/token.h"
#include "src/lexer/lexical_error.h"
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