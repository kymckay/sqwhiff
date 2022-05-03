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

TEST_F(PreprocessorTest, ErrorsOnMismatchedMacroArguments)
{
    ASSERT_EXCEPTION(preprocess("#define _M(A) A\n_M(1,2)"), PreprocessingError, "2:1 PreprocessingError - Invalid number of macro arguments for '_M' supplied, found 2, expected 1");
}

TEST_F(PreprocessorTest, DISABLED_ErrorsAtPositionInMacroBody)
{
    ASSERT_EXCEPTION(preprocess("#define _M1(A) A\n#define _M2 _M1(2\n_M2"), PreprocessingError, "2:13 PreprocessingError - Unclosed macro arguments '_M1('");
}

TEST_F(PreprocessorTest, DISABLED_ErrorsAtPositionInMacroArguments)
{
    ASSERT_EXCEPTION(preprocess("#define _M1(A) A\n_M1(1 + _M1(6)"), PreprocessingError, "2:9 PreprocessingError - Unclosed macro arguments '_M1('");
}

TEST_F(PreprocessorTest, DISABLED_ErrorsOnInvalidUndefineMacroID)
{
    ASSERT_EXCEPTION(preprocess("#undef 3"), PreprocessingError, "1:8 PreprocessingError - Macro ID must start with an alpha character or _, found '3'");
}

// TODO: Check if mixed nesting should work (I suspect not)
TEST_F(PreprocessorTest, ErrorsOnNestedBranching)
{
    ASSERT_EXCEPTION(preprocess("#ifdef ONE\n#ifdef TWO\n#endif\n#endif"), PreprocessingError, "2:1 PreprocessingError - Cannot nest #ifdef directives");
    ASSERT_EXCEPTION(preprocess("#ifndef ONE\n#ifndef TWO\n#endif\n#endif"), PreprocessingError, "2:1 PreprocessingError - Cannot nest #ifndef directives");
}

TEST_F(PreprocessorTest, ErrorsOnLoneElse)
{
    ASSERT_EXCEPTION(preprocess("#else\n#endif"), PreprocessingError, "1:1 PreprocessingError - Cannot use #else with no preceeding #if, #ifdef or #ifndef directive");
}
