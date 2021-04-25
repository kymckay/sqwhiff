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

int Parser::factor()
{
    Token t = current_token_;
    eat(TokenType::number);
    return std::stoi(t.raw);
};

