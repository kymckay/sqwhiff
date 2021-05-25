#pragma once
#include "test/parser/_tester.h"
#include <string>
#include <sstream>

std::string parse(std::string s)
{
    std::stringstream ss(s);
    Preprocessor pp(ss);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    return t.test();
}