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
TEST_F(PreprocessorTest, ErrorsOnInvalidDirective)
{
    // Can't use SQF hash selection operator at line start due to preprocessing
    ASSERT_EXCEPTION(preprocess("[1,2,3]\n #2"), PreprocessingError, "2:2 PreprocessingError - Invalid preprocessor directive, no instruction immediately following the # character");
}

TEST_F(PreprocessorTest, ErrorsOnUnrecognisedDirective)
{
    ASSERT_EXCEPTION(preprocess("#random"), PreprocessingError, "1:1 PreprocessingError - Unrecognised preprocessor directive '#random'");
}

// TODO
TEST_F(PreprocessorTest, DISABLED_ErrorsOnInvalidMacroName)
{
    ASSERT_EXCEPTION(preprocess("#define 3"), PreprocessingError, "1:9 PreprocessingError - Invalid macro name");
}

// TODO
TEST_F(PreprocessorTest, DISABLED_ErrorsOnInvalidMacroParams)
{
    ASSERT_EXCEPTION(preprocess("#define _M(A,B,3)"), PreprocessingError, "1:16 PreprocessingError - Invalid macro parameter");
}

TEST_F(PreprocessorTest, ErrorsOnUnclosedMacroArguments)
{
    ASSERT_EXCEPTION(preprocess("#define _M(A) A\n_M(anythinggoeshere\n"), PreprocessingError, "2:3 PreprocessingError - Unclosed macro arguments '_M('");
}
