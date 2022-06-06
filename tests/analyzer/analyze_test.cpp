#include "tests/analyzer/_test.hpp"

TEST_F(AnalyzerTest, OutputsLowerErrors) {
  // Case with preprocessor error
  analyze("#random");
  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Unrecognised preprocessor directive '#random'", errorMessage(0));
  EXPECT_EQ("PreprocessingError", errorType(0));

  // Case with lexical error
  analyze("?");
  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Unexpected character '?'", errorMessage(0));
  EXPECT_EQ("LexicalError", errorType(0));

  // Case with parsing error
  analyze("[1,");
  EXPECT_EQ(1, errorCount());
  EXPECT_EQ("Unexpected token '', expected a variable, literal or expression",
            errorMessage(0));
  EXPECT_EQ("SyntaxError", errorType(0));
}
