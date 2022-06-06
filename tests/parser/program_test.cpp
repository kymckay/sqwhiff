#include "tests/parser/_test.hpp"

// Issue #10
TEST_F(ParserTest, HandlesProgramWithoutFinalDelimiter) {
  EXPECT_EQ("(allunits),(allunits)", parse("allunits; allunits"))
      << "Final statement can have no delimiter";
}

// Issue #10
TEST_F(ParserTest, HandlesProgramWithFinalDelimiter) {
  EXPECT_EQ("(allunits),(allunits),<NoOp>", parse("allunits; allunits;"))
      << "Final (real) statement can have a delimiter";
}

// Issue #10
TEST_F(ParserTest, HandlesEmptyProgram) {
  EXPECT_EQ("<NoOp>", parse(""))
      << "Program can contain no (real) statements at all";
}
