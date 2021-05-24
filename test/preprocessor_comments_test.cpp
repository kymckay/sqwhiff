#include "src/preprocessor/preprocessor.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(Comments, CanBeInline)
{
    std::stringstream input("selectRandom//comment \"test\"\n[1,2]");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "selectRandom\n[1,2]")
        << "Inline comments should not be in the preprocessor output";
}

TEST(Comments, CanBeBlock)
{
    std::stringstream input("selectRandom /* //?#[20] */ [1,2]");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "selectRandom  [1,2]")
        << "Block comments should not be in the preprocessor output";
}

TEST(Comments, AreRemovedInPreprocessing)
{
    std::stringstream input("select/* why would you do this */Random [1,2]");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "selectRandom [1,2]")
        << "Block comments should not delimit tokens";
}

TEST(Comments, AreConsumedByDoubleQuotes)
{
    std::stringstream input("\"/* this isn't a comment */\"");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\"/* this isn't a comment */\"")
        << "Comments within double quotes should appear in preprocessor output";
}

// The preprocessor ignores single quotes
TEST(Comments, AreRemovedInSingleQuotes)
{
    std::stringstream input("\'/* this is a comment */\'");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "''")
        << "Comments within single quotes should not appear in preprocessor output";
}

// This is consistent with being removed during preprocessing
TEST(Comments, AreRemovedInBalancedQuotes)
{
    std::stringstream input("\"\"/* this is a comment */\"\"");
    Preprocessor pp(input);

    EXPECT_EQ(pp.processAll(), "\"\"\"\"")
        << "Comments within balanced sets of double quotes should not appear in preprocessor output";
}