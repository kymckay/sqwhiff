#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(LexicalError, ThrownOnUnknownChar)
{
    std::stringstream input("?");

    Preprocessor pp(input);
    Lexer l(pp);
    EXPECT_THROW(l.nextToken(), LexicalError) << "Unexpected character throws exception";
}

TEST(LexicalError, ThrownOnUnclosedString)
{
    std::stringstream input("\"");

    Preprocessor pp(input);
    Lexer l(pp);
    EXPECT_THROW(l.nextToken(), LexicalError) << "Unclosed string throws exception";
}

TEST(LexicalError, ThrowOnIncompleteSciNotation)
{
    std::stringstream input("1e+");

    Preprocessor pp(input);
    Lexer l(pp);
    EXPECT_THROW(l.nextToken(), LexicalError) << "Invalid numeric literal throws exception";
}

// TODO check error messages
TEST(LexicalError, ThrownByParser)
{
    // See #16
    std::stringstream immediate("?");
    Preprocessor pp1(immediate);
    Lexer l1(pp1);
    Parser p1(l1);

    EXPECT_THROW(p1.parse(), LexicalError) << "Lexer errors thrown by parser on immediate error";

    std::stringstream otherwise("statement;statement;\n\n\n'this is unclosed");
    Preprocessor pp2(otherwise);
    Lexer l2(pp2);
    Parser p2(l2);

    EXPECT_THROW(p2.parse(), LexicalError) << "Lexer errors thrown by parser elsewhere";
}