#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(LexicalError, ThrownOnUnknownChar)
{
    std::stringstream test_str("?");

    Lexer l(test_str);
    EXPECT_THROW(l.nextToken(), LexicalError) << "Unexpected character throws exception";
}

TEST(LexicalError, ThrownOnUnclosedString)
{
    std::stringstream test_str("\"");

    Lexer l(test_str);
    EXPECT_THROW(l.nextToken(), LexicalError) << "Unclosed string throws exception";
}

TEST(LexicalError, ThrowOnIncompleteSciNotation)
{
    std::stringstream test_str("1e+");

    Lexer l(test_str);
    EXPECT_THROW(l.nextToken(), LexicalError) << "Invalid numeric literal throws exception";
}

// TODO check error messages
TEST(LexicalError, ThrownByParser)
{
    // See #16
    std::stringstream immediate("?");
    Lexer l1(immediate);
    Parser p1(l1);

    EXPECT_THROW(p1.parse(), LexicalError) << "Lexer errors thrown by parser on immediate error";

    std::stringstream otherwise("statement;statement;\n\n\n'this is unclosed");
    Lexer l2(otherwise);
    Parser p2(l2);

    EXPECT_THROW(p2.parse(), LexicalError) << "Lexer errors thrown by parser elsewhere";
}