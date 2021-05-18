#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(Macros, CanBeDefinedEmpty)
{
    std::stringstream input("#define MACRO\n");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<NoOp>") << "Macro definition was not ignored";
}

TEST(Macros, CanBeDefinedWithArgs)
{
    // A trailing comma is valid
    std::stringstream input("#define MACRO(A,B,C,D,)\n");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<NoOp>") << "Macro definition was not ignored";
}

TEST(Macros, CanBeExpandedEmpty)
{
    std::stringstream input("#define MACRO\nMACRO");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<NoOp>") << "Macro token was not expanded to nothing";
}

TEST(Macros, CanBeExpandedToStatic)
{
    std::stringstream input("#define MACRO 1\nMACRO");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<Dec:1>") << "Macro token was not expanded to static content";
}

TEST(Macros, CanBeExpandedWithArgs)
{
    // A trailing comma is valid
    std::stringstream input("#define MACRO(A,B,C,D,)\nMACRO(1,2,3,4)");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "<NoOp>") << "Macro token with args was not expanded to nothing";
}

TEST(Macros, CanBeExpandedWithArgReplacement)
{
    std::stringstream input("#define SEL(A,B,C) A B C\nSEL([1],select,0)");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);
    Tester t(p);

    EXPECT_EQ(t.test(), "([<Dec:1>] select <Dec:0>)") << "Macro token with args was not expanded with replacement";
}