#pragma once
#include "lexer.h"
#include "token.h"
#include "ast.h"
#include <deque>
#include <memory>

class Parser
{
    // Reference member, no need to copy the supplied lexer
    Lexer &lexer_;
    Token current_token_;
    std::deque<Token> peek_buffer_;

    void error();
    void eat(TokenType);
    Token peek(int);

    // Member functions for AST nodes
    // Dynamic allocation is needed for AST interface types

    std::unique_ptr<AST> program();
    std::unique_ptr<AST> statement_list();
    std::unique_ptr<AST> statement();
    std::unique_ptr<AST> assignment();
    std::unique_ptr<AST> expr();
    std::unique_ptr<AST> conjunction();
    std::unique_ptr<AST> comparison();
    std::unique_ptr<AST> binary_op();
    std::unique_ptr<AST> else_op();
    std::unique_ptr<AST> term();
    std::unique_ptr<AST> factor();
    std::unique_ptr<AST> power();
    std::unique_ptr<AST> hash_select();
    std::unique_ptr<AST> unary_op();
    std::unique_ptr<AST> nullary_op();
    std::unique_ptr<AST> atom();
    std::unique_ptr<AST> code();
    std::unique_ptr<AST> variable();
    std::unique_ptr<AST> empty();

public:
    Parser(Lexer &);
    std::unique_ptr<AST> parse();
};
