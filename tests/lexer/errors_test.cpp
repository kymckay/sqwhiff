#include <gmock/gmock.h>

#include <string>

#include "tests/lexer/_test.hpp"

using sqwhiff::LexicalError;

TEST_F(LexerTest, NoErrorOnInstantiation) {
  EXPECT_NO_THROW(std::stringstream ss("#random"); Preprocessor pp(ss);
                  Lexer l(pp);)
      << "Lexer instantiation logic should not expose preprocessing exceptions";
}

TEST_F(LexerTest, ErrorsOnUnexpectedChar) {
  EXPECT_THAT([this] { tokenize("?"); }, ::testing::ThrowsMessage<LexicalError>(
                                             "Unexpected character '?'"));
}

TEST_F(LexerTest, ErrorsOnUnclosedStr) {
  EXPECT_THAT([this] { tokenize("\""); },
              ::testing::ThrowsMessage<LexicalError>("Unclosed string"));
}

TEST_F(LexerTest, ErrorsOnIncompleteSciNotation) {
  EXPECT_THAT([this] { tokenize("1e+"); },
              ::testing::ThrowsMessage<LexicalError>(
                  "Unfinished numeric literal '1e+'"));
}
