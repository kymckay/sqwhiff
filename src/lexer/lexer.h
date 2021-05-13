#pragma once
#include "src/preprocessor/preprocessor.h"
#include "src/lexer/token.h"
#include "src/lexer/lexical_error.h"
#include <string>
#include <istream>

class Lexer
{
    // Reference member, no need to copy the supplied preprocessor
    Preprocessor &preproc_;

    PosChar current_char_;

    void error(Token, std::string);
    char peek();
    void advance();
    void skipWhitespace();
    Token _id();
    Token number();
    Token string();
    Token makeToken(TokenType, std::string);

public:
    Lexer(Preprocessor&);
    Token nextToken();
};