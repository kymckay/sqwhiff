#include "sqwhiff/preprocessor/source_consumer.hpp"

#include <gtest/gtest.h>

#include <sstream>

using sqwhiff::SourceChar;
using sqwhiff::SourceConsumer;

TEST(SourceConsumer, ReportsCorrectChar) {
  std::stringstream source("123");
  SourceConsumer consumer(source);

  EXPECT_EQ('1', consumer.advance().character);
  EXPECT_EQ('2', consumer.advance().character);
  EXPECT_EQ('3', consumer.advance().character);
  EXPECT_EQ('\0', consumer.advance().character);
  EXPECT_EQ('\0', consumer.advance().character)
      << "Multiple advances past EOF should keep returning the same character";
}

TEST(SourceConsumer, ReportsCorrectColumn) {
  std::stringstream source("123\n4");
  SourceConsumer consumer(source);

  EXPECT_EQ(1, consumer.advance().column) << "The column should be 1-indexed";
  EXPECT_EQ(2, consumer.advance().column);
  EXPECT_EQ(3, consumer.advance().column);
  EXPECT_EQ(4, consumer.advance().column);
  EXPECT_EQ(1, consumer.advance().column)
      << "After a new line the column should reset to 1";
  EXPECT_EQ(2, consumer.advance().column);
  EXPECT_EQ(2, consumer.advance().column)
      << "Multiple advances past EOF should keep returning the same character";
}

TEST(SourceConsumer, ReportsCorrectLine) {
  std::stringstream source("1\n2\n3\n4");
  SourceConsumer consumer(source);

  EXPECT_EQ(1, consumer.advance().line) << "The line should be 1-indexed";
  EXPECT_EQ(1, consumer.advance().line);

  EXPECT_EQ(2, consumer.advance().line);
  EXPECT_EQ(2, consumer.advance().line);

  EXPECT_EQ(3, consumer.advance().line);
  EXPECT_EQ(3, consumer.advance().line);

  EXPECT_EQ(4, consumer.advance().line);
  EXPECT_EQ(4, consumer.advance().line)
      << "EOF considered to be on the final line";
  EXPECT_EQ(4, consumer.advance().line)
      << "Multiple advances past EOF should keep returning the same character";
}

TEST(SourceConsumer, PeeksCorrectChar) {
  std::stringstream source("1\n2");
  SourceConsumer consumer(source);

  EXPECT_EQ('1', consumer.peek().character);
  consumer.advance();
  EXPECT_EQ('\n', consumer.peek().character);
  consumer.advance();
  EXPECT_EQ('2', consumer.peek().character);
  consumer.advance();
  EXPECT_EQ('\0', consumer.peek().character)
      << "Peeking beyond final character should reflect EOF";
  consumer.advance();
  EXPECT_EQ('\0', consumer.peek().character)
      << "Peeking beyond EOF should keep returning EOF";
}

TEST(SourceConsumer, PeeksCorrectColumn) {
  std::stringstream source("1\n2");
  SourceConsumer consumer(source);

  EXPECT_EQ(1, consumer.peek().column);
  consumer.advance();
  EXPECT_EQ(2, consumer.peek().column);
  consumer.advance();
  EXPECT_EQ(1, consumer.peek().column)
      << "Peeking beyond a new line should report a reset column index";
  consumer.advance();
  EXPECT_EQ(2, consumer.peek().column);
  consumer.advance();
  EXPECT_EQ(2, consumer.peek().column);
}

TEST(SourceConsumer, PeeksCorrectLine) {
  std::stringstream source("1\n2");
  SourceConsumer consumer(source);

  EXPECT_EQ(1, consumer.peek().line);
  consumer.advance();
  EXPECT_EQ(1, consumer.peek().line);
  consumer.advance();
  EXPECT_EQ(2, consumer.peek().line)
      << "Peeking beyond a new line should report the next line";
  consumer.advance();
  EXPECT_EQ(2, consumer.peek().line);
  consumer.advance();
  EXPECT_EQ(2, consumer.peek().line);
}
