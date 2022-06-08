#include "sqwhiff/preprocessor/source_consumer.hpp"

namespace sqwhiff {

SourceConsumer::SourceConsumer(std::istream& source, fs::path file)
    : source_(source) {
  current_char_.character = '\0';
  current_char_.line = 1;
  current_char_.column = 0;
  current_char_.file = file;
};

SourceChar SourceConsumer::advance() {
  // Increment the line whenever a newline is passed
  if (current_char_ == '\n') {
    current_char_.line++;
    current_char_.column = 0;
  }

  // Stop advancing once EOF reached
  if (source_.good()) {
    current_char_.column++;
  }

  if (source_.get(current_char_.character).fail()) {
    current_char_.character = '\0';
  }

  return current_char_;
}

SourceChar SourceConsumer::peek() {
  SourceChar next = current_char_;

  if (next == '\n') {
    next.line++;
    next.column = 1;
  } else {
    next.column++;
  }

  next.character = source_.peek();
  if (next.character == EOF) {
    next.character = '\0';
  }

  return next;
}

SourceChar SourceConsumer::at() { return current_char_; }

}  // namespace sqwhiff
