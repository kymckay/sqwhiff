#pragma once
#include "lexer.h"
#include "token.h"
#include <string>

class Parser
{
    // Reference member, no need to copy the supplied lexer
    Lexer &lexer_;
    Token current_token_;

    void error();
    void eat(TokenType);
    int term();
    int factor();

public:
    Parser(Lexer &);
    int expr();
};
