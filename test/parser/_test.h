#pragma once
#include "test/parser/_tester.h"
#include <gtest/gtest.h>
#include <string>
#include <sstream>

class ParserTest : public ::testing::Test
{
protected:
    std::string parse(std::string s)
    {
        std::stringstream ss(s);
        Preprocessor pp(ss);
        Lexer l(pp);
        Parser p(l);
        Tester t(p);

        return t.test();
    }
};