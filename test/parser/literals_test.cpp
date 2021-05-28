#include "./_test.h"

// Issue #18
TEST_F(ParserTest, HandlsEmptyArrayDisplay)
{
    EXPECT_EQ("[]", parse("[]")) << "Empty array literals should be parsable";
}

// Issue #18
TEST_F(ParserTest, HandlesEmptyCodeDisplay)
{
    EXPECT_EQ("{<NoOp>}", parse("{}")) << "Empty code literals should be parsable";
}