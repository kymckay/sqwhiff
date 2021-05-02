#include "src/lexer/lexer.h"
#include "src/tokens/token.h"
#include "src/parser/parser.h"
#include "src/tester/tester.h"
#include <iostream>
#include <fstream>

int main()
{
    std::ifstream file_in("test.txt");
    Lexer lex(file_in);
    Parser p(lex);
    Tester t(p);
    t.test();
}
