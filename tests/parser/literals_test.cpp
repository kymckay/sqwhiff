#include "tests/parser/_test.hpp"

// Issue #18
TEST_F(ParserTest, HandlsEmptyArrayDisplay) {
  EXPECT_EQ("[]", parse("[]")) << "Empty array literals should be parsable";
}

// Issue #18
TEST_F(ParserTest, HandlesEmptyCodeDisplay) {
  EXPECT_EQ("{<NoOp>}", parse("{}"))
      << "Empty code literals should be parsable";
}

TEST_F(ParserTest, HandlesCodeDisplayNoOp) {
  EXPECT_EQ("{<Dec:1>,<NoOp>}", parse("{1;}"))
      << "Code literals can end with a final NoOp statement";
}
