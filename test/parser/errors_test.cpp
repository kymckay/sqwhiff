#include "test/parser/_test.h"
#include <gtest/gtest.h>
#include <string>

#define ASSERT_EXCEPTION(TO_TRY, EXCEPTION, MSG)   \
    try                                            \
    {                                              \
        TO_TRY;                                    \
        FAIL() << "Expected an exception of type " \
               << #EXCEPTION;                      \
    }                                              \
    catch (EXCEPTION const &err)                   \
    {                                              \
        EXPECT_EQ(std::string(MSG), err.what())     \
            << "Exception message is incorrect";   \
    }

TEST(SyntaxError, ThrownOnUnclosedArray)
{
    ASSERT_EXCEPTION(parse("[1,2,3,4,5"), SyntaxError, "1:10 SyntaxError - Unexpected token '', expected a closing square bracket");
}

TEST(SyntaxError, ThrownOnUnclosedCode)
{
    ASSERT_EXCEPTION(parse("[1] select {_x == 1"), SyntaxError, "1:19 SyntaxError - Unexpected token '', expected a closing curly bracket");
}