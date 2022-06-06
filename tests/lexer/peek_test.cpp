#include "tests/lexer/_test.hpp"

TEST_F(LexerTest, CanPeekMultiple) {
  EXPECT_EQ("6", peek("1 2 3 4 5 6", 6).raw)
      << "Peeking ahead should return the token the corresponding number of "
         "indexes ahead";
}
