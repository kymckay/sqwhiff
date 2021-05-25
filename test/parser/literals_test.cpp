#include "./_test.h"

// Issue #18
TEST(ArrayDisplay, CanBeEmpty)
{
    EXPECT_EQ("[]", parse("[]")) << "Empty array literals should be parsable";
}

// Issue #18
TEST(CodeDisplay, CanBeEmpty)
{
    EXPECT_EQ("{<NoOp>}", parse("{}")) << "Empty code literals should be parsable";
}