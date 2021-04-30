#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "ast.h"
#include "compound.h"
#include "no_op.h"
#include "nullary_op.h"
#include "unary_op.h"
#include "binary_op.h"
#include "assign.h"
#include "variable.h"
#include "number.h"
#include "string_literal.h"
#include <memory>
#include <utility>
#include <vector>
#include <iostream>

// Consume a file's tokens and structure them together into an AST (parse the file)
Parser::Parser(Lexer &lexer) : lexer_(lexer)
{
    current_token_ = lexer_.nextToken();
}

void Parser::error()
{
    throw "Invalid syntax";
}

void Parser::eat(TokenType type)
{
    if (current_token_.type == type)
    {
        current_token_ = lexer_.nextToken();
    }
    else
    {
        std::cout << "mismatched token: " << current_token_.raw;
        error();
    }
}

std::unique_ptr<AST> Parser::program()
{
    std::unique_ptr<AST> node = statement_list();
    eat(TokenType::end_of_file);
    return node;
}

std::unique_ptr<AST> Parser::statement_list()
{
    std::vector<std::unique_ptr<AST>> results;
    results.push_back(statement());

    while (
        current_token_.type == TokenType::semi || current_token_.type == TokenType::comma)
    {
        eat(current_token_.type);
        results.push_back(statement());
    }

    return std::unique_ptr<AST>(new Compound(std::move(results)));
}

std::unique_ptr<AST> Parser::statement()
{
    if (current_token_.type == TokenType::id)
    {
        return assignment_statement();
    }
    else
    {
        return empty();
    }
}

std::unique_ptr<AST> Parser::assignment_statement()
{
    std::unique_ptr<AST> left = variable();
    Token t = current_token_;
    eat(TokenType::assign);

    return std::unique_ptr<AST>(new Assign(std::move(left), t, expr()));
}

std::unique_ptr<AST> Parser::variable(){
    std::unique_ptr<AST> node = std::unique_ptr<AST>(new Variable(current_token_));
    eat(TokenType::id);
    return node;
}

// expr: term ((PLUS|MINUS) term)*
std::unique_ptr<AST> Parser::expr()
{
    std::unique_ptr<AST> node = term();

    while (
        current_token_.type == TokenType::plus || current_token_.type == TokenType::minus)
    {
        Token t = current_token_;
        eat(t.type);
        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, term()));
    }

    return node;
}

// term: factor ((MUL|DIV) factor)*
std::unique_ptr<AST> Parser::term()
{
    std::unique_ptr<AST> node = unary_op();

    while (
        current_token_.type == TokenType::div || current_token_.type == TokenType::mul)
    {
        Token t = current_token_;
        if (t.type == TokenType::div)
        {
            eat(TokenType::div);
        }
        else
        {
            eat(TokenType::mul);
        }

        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, unary_op()));
    }

    return node;
}

// unary_op : (PLUS|MINUS|NEGATION|KEYWORD)factor | nullary_op
std::unique_ptr<AST> Parser::unary_op()
{
    Token t = current_token_;
    switch (t.type)
    {
    case TokenType::plus:
    case TokenType::minus:
    case TokenType::negation:
    case TokenType::keyword:
    {
        eat(t.type);
        return std::unique_ptr<AST>(new UnaryOp(t, unary_op()));
    }
    default:
        return nullary_op();
    }
}

// nullary_op : KEYWORD | LPAREN expr RPAREN | atom
std::unique_ptr<AST> Parser::nullary_op()
{
    Token t = current_token_;
    switch (t.type)
    {
    case TokenType::keyword:
    {
        return std::unique_ptr<AST>(new NullaryOp(t));
    }
    case TokenType::lparen:
    {
        eat(TokenType::lparen);
        std::unique_ptr<AST> node = expr();
        eat(TokenType::rparen);
        return node;
    }
    default:
        return atom();
    }
}

// atom : STR_LITERAL | HEX_LITERAL | DEC_LITERAL | array | code | variable
std::unique_ptr<AST> Parser::atom()
{
    Token t = current_token_;
    switch (t.type)
    {
    case TokenType::dec_literal:
    case TokenType::hex_literal:
    {
        eat(t.type);
        return std::unique_ptr<AST>(new Number(t));
    }
    case TokenType::str_literal:
    {
        eat(t.type);
        return std::unique_ptr<AST>(new StringLiteral(t));
    }
    case TokenType::lsqb:
    {
        // TODO array displays
    }
    case TokenType::lcurl:
    {
        // TODO code displays
    }
    default:
        return variable();
    }
}

std::unique_ptr<AST> Parser::empty(){
    return std::unique_ptr<AST>(new NoOp());
}

// Parses the file and returns the root node of the AST
std::unique_ptr<AST> Parser::parse()
{
    std::unique_ptr<AST> node = program();

    if (current_token_.type != TokenType::end_of_file)
        error();

    return node;
};