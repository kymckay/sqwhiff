#include "./_test.h"

TEST_F(LexerTest, KeywordsAreCaseInsensitive) {
  tokenize("aLLuNiTs cOmMeNt SelECT PRIvate");

  EXPECT_EQ(TokenType::nullary, getType(0))
      << "Nullary tokens should be identified regardless of case";

  EXPECT_EQ(TokenType::keyword, getType(1))
      << "Unary keywords should be identified regardless of case";

  EXPECT_EQ(TokenType::keyword, getType(2))
      << "Binary keywords should be identified regardless of case";

  EXPECT_EQ(TokenType::private_op, getType(3))
      << "Private keyword should be identified regardless of case";
}
