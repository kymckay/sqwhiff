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

TEST_F(PreprocessorTest, ErrorsOnInvalidMacroID)
{
    ASSERT_EXCEPTION(preprocess("#define 3 body"), PreprocessingError, "1:9 PreprocessingError - Macro ID must start with an alpha character or _, found '3'");
}

TEST_F(PreprocessorTest, ErrorsOnInvalidMacroParamID)
{
    ASSERT_EXCEPTION(preprocess("#define _M(A,B,3) body"), PreprocessingError, "1:16 PreprocessingError - Macro parameter ID must start with an alpha character or _, found '3'");
}

TEST_F(PreprocessorTest, ErrorsOnUnclosedMacroArguments)
{
    ASSERT_EXCEPTION(preprocess("#define _M(A) A\n_M(anythinggoeshere\n"), PreprocessingError, "2:3 PreprocessingError - Unclosed macro arguments '_M('");
}
