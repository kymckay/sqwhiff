#include "tests/preprocessor/_test.hpp"

// Escaping a newline continues the directive seamlessly
TEST_F(PreprocessorTest, SupportsExtendedLogicalLine) {
  // Can even be extended mid token without delimiting
  EXPECT_EQ("\na", preprocess("#\\\ndef\\\nine MA\\\nCRO 2\\\n1\na"))
      << "Extended directive logical line should not be included in "
         "preprocessor output";
}

TEST_F(PreprocessorTest, IgnoresHashCharMidLine) {
  EXPECT_EQ("[1] # 0", preprocess("[1] # 0"))
      << "The hash character should only be a directive at line start";
}

// TODO: Check if whitespace before directive should appear in output
TEST_F(PreprocessorTest, IgnoresWhitespaceAtLineStart) {
  EXPECT_EQ("     ", preprocess("     #define A"))
      << "A directive can be preceded by whitespace";
}
