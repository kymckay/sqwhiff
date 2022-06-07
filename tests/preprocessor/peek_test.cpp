#include "tests/preprocessor/_test.hpp"

TEST_F(PreprocessorTest, CanPeekMultiple) {
  EXPECT_EQ('6', (char)peek("123456", 6))
      << "Peeking ahead should return the character the corresponding number "
         "of indexes ahead";
}
