#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>
#include <string>

#define ASSERT_EXCEPTION(TO_TRY, EXCEPTION, MSG)   \
    try                                            \
    {                                              \
        TO_TRY                                     \
        FAIL() << "Expected an exception of type " \
        << #EXCEPTION;                             \
    }                                              \
    catch (EXCEPTION const &err)                   \
    {                                              \
        EXPECT_EQ(err.what(), std::string(MSG))    \
        << "Exception message is incorrect";       \
    }

TEST(LexicalError, ThrownOnUnknownChar)
{
    std::stringstream input("?");

    Preprocessor pp(input);
    Lexer l(pp);

    ASSERT_EXCEPTION(l.nextToken();, LexicalError, "1:1 LexicalError - Unexpected character '?'");
}

TEST(LexicalError, ThrownOnUnclosedString)
{
    std::stringstream input("\"");

    Preprocessor pp(input);
    Lexer l(pp);

    ASSERT_EXCEPTION(l.nextToken();, LexicalError, "1:1 LexicalError - Unclosed string");
}

TEST(LexicalError, ThrowOnIncompleteSciNotation)
{
    std::stringstream input("1e+");

    Preprocessor pp(input);
    Lexer l(pp);

    ASSERT_EXCEPTION(l.nextToken();, LexicalError, "1:1 LexicalError - Unfinished numeric literal '1e+'");
}

TEST(LexicalError, ThrownByParser)
{
    // See #16
    std::stringstream immediate("?");
    Preprocessor pp1(immediate);
    Lexer l1(pp1);
    Parser p1(l1);

    EXPECT_THROW(p1.parse(), LexicalError) << "Immediate lexical error was not thrown out of parser";

    std::stringstream otherwise("statement;statement;\n\n\n'this is unclosed");
    Preprocessor pp2(otherwise);
    Lexer l2(pp2);
    Parser p2(l2);

    EXPECT_THROW(p2.parse(), LexicalError) << "Lexical error was not thrown out of parser during parsing";
}