#include "src/preprocessor/preprocessor.h"
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
        Preprocessor preproc(file_in);
        Lexer lex(preproc);
        Parser p(lex);
        Analyzer a(p);
        a.analyze(std::cout);
        file_in.close();
    }
    else
    {
        std::cout << "Unable to open file";
    }
}
