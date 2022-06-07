#include "sqwhiff/structures/source_char.hpp"

#include <gtest/gtest.h>

using sqwhiff::SourceChar;

TEST(SourceChar, ConvertsToChar) {
  SourceChar content;
  content.character = 'a';

  EXPECT_EQ('a', (char)content);
}

TEST(SourceChar, ComparesToChar) {
  SourceChar content;
  content.character = 'b';

  EXPECT_TRUE(content == 'b');
}

TEST(SourceChar, AssignsToChar) {
  SourceChar content;
  content = 'c';

  EXPECT_EQ('c', content.character);
}
