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

TEST(SourceString, CanAppend) {
  SourceChar next_char;

  // Populating the original source string
  SourceString content;
  for (auto &&c : {'a', 'b'}) {
    next_char.character = c;
    content.chars.push_back(next_char);
  }

  // Populating the additional source string
  SourceString addition;
  for (auto &&c : {'c', 'd'}) {
    next_char.character = c;
    addition.chars.push_back(next_char);
  }

  content.append(addition);
  EXPECT_EQ("abcd", (std::string)content);
}
