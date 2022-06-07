#include "sqwhiff/structures/source_string.hpp"

#include <gtest/gtest.h>

using sqwhiff::SourceChar;
using sqwhiff::SourceString;

TEST(SourceString, ConvertsToEmptyString) {
  SourceString content;

  EXPECT_EQ("", (std::string)content);
}

TEST(SourceString, ConvertsToFilledString) {
  SourceString content;
  SourceChar next_char;

  // Populating the source string
  for (auto &&c : {'a', 'b', 'c', 'd'}) {
    next_char.character = c;
    content.chars.push_back(next_char);
  }

  EXPECT_EQ("abcd", (std::string)content);
}
