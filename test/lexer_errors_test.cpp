#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>
#include <exception>

TEST(LexerError, ThrownOnUnknownChar)
{
    std::stringstream test_str("?");

    Lexer l(test_str);
    EXPECT_THROW(l.nextToken(), std::runtime_error) << "Unexpected character throws exception";
}

TEST(LexerError, ThrownOnUnclosedString)
{
    std::stringstream test_str("\"");

    Lexer l(test_str);
    EXPECT_THROW(l.nextToken(), std::runtime_error) << "Unclosed string throws exception";
}

TEST(LexerError, ThrowOnIncompleteSciNotation)
{
    std::stringstream test_str("1e+");

    Lexer l(test_str);
    EXPECT_THROW(l.nextToken(), std::runtime_error) << "Invalid numeric literal throws exception";
}

// TODO This test is imperfect, but will tweak once reworking error logic
TEST(LexerError, CaughtByParser)
{
    // See #16
    std::stringstream immediate("?");
    Lexer l1(immediate);
    Parser p1(l1);

    EXPECT_THROW(p1.parse(), std::runtime_error) << "Lexer errors caught by parser on immediate error";

    std::stringstream otherwise("statement;statement;\n\n\n'this is unclosed");
    Lexer l2(otherwise);
    Parser p2(l2);

    EXPECT_THROW(p2.parse(), std::runtime_error) << "Lexer errors caught by parser elsewhere";
}