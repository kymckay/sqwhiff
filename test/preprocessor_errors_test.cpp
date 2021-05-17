#include "test/tester.h"
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

TEST(PreprocessingError, ThrownOnBadDirective)
{
    // Can't use hash selection operator at line start due to preprocessing
    std::stringstream input("[1,2,3]\n #2");

    Preprocessor pp(input);

    // Skip non-problematic characters
    for (size_t i = 0; i < 9; i++)
    {
        pp.get();
    }

    // The 2 is not in the message since directives are only alpha characters
    ASSERT_EXCEPTION(pp.get(), PreprocessingError, "2:2 PreprocessingError - Unrecognised preprocessor directive '#2'");
}