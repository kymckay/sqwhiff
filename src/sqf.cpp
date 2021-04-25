#include "lexer.h"
#include "token.h"
#include "parser.h"
#include <iostream>
#include <fstream>

int main()
{
    std::ifstream file_in("test.txt");
    Lexer lex(file_in);
    Parser par(lex);
    par.parse();
}