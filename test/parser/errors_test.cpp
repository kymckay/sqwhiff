#include <gmock/gmock.h>

#include <string>

#include "./_test.h"

using sqwhiff::LexicalError;
using sqwhiff::SyntaxError;

TEST_F(ParserTest, ErrorsOnUnclosedArray) {
  // 1, 10
  EXPECT_THAT([this] { parse("[1,2,3,4,5"); },
              ::testing::ThrowsMessage<SyntaxError>(
                  "Unexpected token '', expected a closing "
                  "square bracket"));
}

TEST_F(ParserTest, ErrorsOnUnclosedCode) {
  // 1, 19
  EXPECT_THAT([this] { parse("[1] select {_x == 1"); },
              ::testing::ThrowsMessage<SyntaxError>(
                  "Unexpected token '', expected a closing "
                  "curly bracket"));
}

TEST_F(ParserTest, DISABLED_ThrownByParser) {
  // See #16
  std::stringstream immediate("?");
  Preprocessor pp1(immediate);
  Lexer l1(pp1);
  Parser p1(l1);

  EXPECT_THROW(p1.parse(), LexicalError)
      << "Immediate lexical error was not thrown out of parser";

  std::stringstream otherwise("statement;statement;\n\n\n'this is unclosed");
  Preprocessor pp2(otherwise);
  Lexer l2(pp2);
  Parser p2(l2);

  EXPECT_THROW(p2.parse(), LexicalError)
      << "Lexical error was not thrown out of parser during parsing";
}
