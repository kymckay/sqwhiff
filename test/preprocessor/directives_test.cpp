#include "./_test.h"
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
        EXPECT_EQ(std::string(MSG), err.what())    \
            << "Exception message is incorrect";   \
    }

// Any line starting with # (ignoring leading spaces) is taken to be a directive
TEST(Directives, ErrorsOnInvalid)
{
    // Can't use SQF hash selection operator at line start due to preprocessing
    ASSERT_EXCEPTION(preprocess("[1,2,3]\n #2"), PreprocessingError, "2:2 PreprocessingError - Invalid preprocessor directive");
}

TEST(Directives, ErrorsOnUnrecognised)
{
    ASSERT_EXCEPTION(preprocess("#random"), PreprocessingError, "1:1 PreprocessingError - Unrecognised preprocessor directive '#random'");
}

// Escaping a newline continues the directive seamlessly
TEST(Directives, SupportsExtendedLogicalLine)
{
    // Can even be extended mid token without delimiting
    EXPECT_EQ(
        "\na",
        preprocess("#\\\ndef\\\nine MA\\\nCRO 2\\\n1\na")
    )
    << "Extended directive logical line should not be included in preprocessor output";
}