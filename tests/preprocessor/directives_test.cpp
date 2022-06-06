#include "tests/preprocessor/_test.hpp"

// Escaping a newline continues the directive seamlessly
TEST_F(PreprocessorTest, SupportsExtendedLogicalLine) {
  // Can even be extended mid token without delimiting
  EXPECT_EQ("\na", preprocess("#\\\ndef\\\nine MA\\\nCRO 2\\\n1\na"))
      << "Extended directive logical line should not be included in "
         "preprocessor output";
}
