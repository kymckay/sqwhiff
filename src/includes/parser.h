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

    std::unique_ptr<AST> program();
    std::unique_ptr<AST> statement_list();
    std::unique_ptr<AST> statement();
    std::unique_ptr<AST> assignment_statement();
    std::unique_ptr<AST> variable();
    std::unique_ptr<AST> expr();
    std::unique_ptr<AST> term();
    std::unique_ptr<AST> unary_op();
    std::unique_ptr<AST> nullary_op();
    std::unique_ptr<AST> atom();
    std::unique_ptr<AST> empty();

public:
    Parser(Lexer &);
    std::unique_ptr<AST> parse();
};
