#include "src/preprocessor/preprocessor.h"
#include <gtest/gtest.h>
#include <sstream>
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
        EXPECT_EQ(err.what(), std::string(MSG))    \
            << "Exception message is incorrect";   \
    }

// Any line starting with # (ignoring leading spaces) is taken to be a directive
TEST(Directive, ErrorsOnInvalid)
{
    // Can't use SQF hash selection operator at line start due to preprocessing
    std::stringstream input("[1,2,3]\n #2");

    Preprocessor pp(input);

    // Skip non-problematic characters
    for (size_t i = 0; i < 9; i++)
    {
        pp.get();
    }

    ASSERT_EXCEPTION(pp.get(), PreprocessingError, "2:2 PreprocessingError - Invalid preprocessor directive");
}

TEST(Directive, ErrorsOnUnrecognised)
{
    std::stringstream input("#random");
    Preprocessor pp(input);
    ASSERT_EXCEPTION(pp.get(), PreprocessingError, "1:1 PreprocessingError - Unrecognised preprocessor directive '#random'");
}

// escaping a newline continues the directive seamlessly
TEST(Directive, SupportsExtendedLogicalLine)
{
    // Can even be extended mid token without delimiting
    std::stringstream input("#\\\ndef\\\nine MA\\\nCRO 2\\\n1\na");

    Preprocessor pp(input);

    // Should return the newline following extended definition
    EXPECT_EQ(pp.get(), '\n');
    EXPECT_EQ(pp.get(), 'a');
}