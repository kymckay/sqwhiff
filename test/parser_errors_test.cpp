#include "test/tester.h"
#include <gtest/gtest.h>
#include <sstream>
#include <string>

#define ASSERT_EXCEPTION(TO_TRY, EXCEPTION, MSG)   \
    try                                            \
    {                                              \
        TO_TRY                                     \
        FAIL() << "Expected an exception of type " \
               << #EXCEPTION;                      \
    }                                              \
    catch (EXCEPTION const &err)                   \
    {                                              \
        EXPECT_EQ(err.what(), std::string(MSG))    \
            << "Exception message is incorrect";   \
    }

TEST(SyntaxError, ThrownOnUnclosedArray)
{
    std::stringstream input("[1,2,3,4,5");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);

    ASSERT_EXCEPTION(p.parse();, SyntaxError, "1:10 SyntaxError - Unexpected token '', expected a closing square bracket");
}

TEST(SyntaxError, ThrownOnUnclosedCode)
{
    std::stringstream input("[1] select {_x == 1");
    Preprocessor pp(input);
    Lexer l(pp);
    Parser p(l);

    ASSERT_EXCEPTION(p.parse();, SyntaxError, "1:19 SyntaxError - Unexpected token '', expected a closing curly bracket");
}