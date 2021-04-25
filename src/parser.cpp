#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "ast.h"
#include "number.h"
#include "binary_op.h"
#include <memory>
#include <utility>

// Consume a file's tokens and structure them together into an AST (parse the file)
Parser::Parser(Lexer &lexer) : lexer_(lexer)
{
    current_token_ = lexer_.nextToken();
}

void Parser::error()
{
    throw "Invalid syntax";
};

void Parser::eat(TokenType type)
{
    if (current_token_.type == type)
    {
        current_token_ = lexer_.nextToken();
    }
    else
    {
        error();
    }
};

// Interprets a numerical term
// factor: NUMBER | LPAREN expr RPAREN
std::unique_ptr<AST> Parser::factor()
{
    Token t = current_token_;
    if (t.type == TokenType::number)
    {
        eat(TokenType::number);
        return std::unique_ptr<AST>(new Number(t));
    }
    else
    {
        eat(TokenType::lparen);
        std::unique_ptr<AST> node = expr();
        eat(TokenType::rparen);
        return node;
    }
};

// Interprets a mul or div factor
// term: factor ((MUL|DIV) factor)*
std::unique_ptr<AST> Parser::term()
{
    std::unique_ptr<AST> node = factor();

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

        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, factor()));
    }

    return node;
};

// Interprets an arithmetic expression
// expr: term ((PLUS|MINUS) term)*
std::unique_ptr<AST> Parser::expr()
{
    std::unique_ptr<AST> node = term();

    while (
        current_token_.type == TokenType::plus || current_token_.type == TokenType::minus)
    {
        Token t = current_token_;
        if (t.type == TokenType::plus)
        {
            eat(TokenType::plus);
        }
        else
        {
            eat(TokenType::minus);
        }

        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, term()));
    }

    return node;
};