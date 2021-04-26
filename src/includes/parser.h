#pragma once
#include "lexer.h"
#include "token.h"
#include "ast.h"
#include <memory>

class Parser
{
    // Reference member, no need to copy the supplied lexer
    Lexer &lexer_;
    Token current_token_;

    void error();
    void eat(TokenType);

    // Member functions for AST nodes
    // Dynamic allocation is needed for AST interface types

    std::unique_ptr<AST> expr();
    std::unique_ptr<AST> term();
    std::unique_ptr<AST> factor();

public:
    Parser(Lexer &);
    std::unique_ptr<AST> parse();
};
