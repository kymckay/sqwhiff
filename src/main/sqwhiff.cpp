#include "src/lexer/lexer.h"
#include "src/parser/parser.h"
#include "src/analyzer/analyzer.h"
#include <iostream>
#include <fstream>

int main()
{
    std::ifstream file_in("test.txt");

    if (file_in.is_open())
    {
        Lexer lex(file_in);
        Parser p(lex);
        Analyzer a(p);
        a.analyze();
        file_in.close();

        a.logErrors(std::cout);
    }
    else
    {
        std::cout << "Unable to open file";
    }
}
