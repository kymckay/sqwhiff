#include "lexer.h"
#include "token.h"
#include "parser.h"
#include <iostream>
#include <fstream>

int main()
{
    std::ifstream file_in("test.txt");
    Lexer test(file_in);
    Token t;
    do
    {
        t = test.nextToken();
        switch (t.type)
        {
        case TokenType::plus:
            std::cout << "plus ";
            break;
        case TokenType::minus:
            std::cout << "minus ";
            break;
        case TokenType::number:
            std::cout << "number ";
            break;
        case TokenType::end_of_file:
            std::cout << "eof ";
            break;
        default:
            std::cout << "unknown ";
            break;
        }
        std::cout << t.line << ": " << t.raw << "\n";
    } while (t.type != TokenType::end_of_file);
}