#include "parser.h"
#include "lexer.h"
#include "token.h"
#include <string>

// Consume a file's tokens and structure them together (parse the file)
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
// factor: NUMBER
int Parser::factor()
{
    Token t = current_token_;
    eat(TokenType::number);
    return std::stoi(t.raw);
};

// Interprets a mul or div factor
// term: factor ((MUL|DIV) factor)*
int Parser::term()
{
    int result = factor();

    while (
        current_token_.type == TokenType::div || current_token_.type == TokenType::mul)
    {
        Token t = current_token_;
        if (t.type == TokenType::div)
        {
            eat(TokenType::div);
            result /= factor();
        }
        else
        {
            eat(TokenType::mul);
            result *= factor();
        }
    }

    return result;
};

// Interprets an arithmetic expression
// expr: term ((PLUS|MINUS) term)*
int Parser::expr()
{
    int result = term();

    while (
        current_token_.type == TokenType::plus || current_token_.type == TokenType::minus)
    {
        Token t = current_token_;
        if (t.type == TokenType::plus)
        {
            eat(TokenType::plus);
            result += term();
        }
        else
        {
            eat(TokenType::minus);
            result -= term();
        }
    }

    return result;
};