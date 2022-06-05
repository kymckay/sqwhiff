#include "./lexer.h"

#include <algorithm>
#include <cctype>
#include <istream>
#include <string>

#include "../sqf/all_keywords.h"
#include "./token_maps.h"

Lexer::Lexer(Preprocessor &pre) : preproc_(pre) {}

void Lexer::error(Token t, std::string msg) {
  throw LexicalError(t.line, t.column, t.file, msg);
}

void Lexer::advance() { current_char_ = preproc_.get(); }

void Lexer::skipWhitespace() {
  while (current_char_ != '\0' && std::isspace(current_char_)) {
    advance();
  }
}

Token Lexer::_id() {
  // Token position at first character
  Token t = makeToken(TokenType::unknown, "");

  while (current_char_ != '\0' &&
         (std::isalnum(current_char_) || current_char_ == '_')) {
    // SQF is not case sensitive
    t.raw.push_back(std::tolower(current_char_));
    advance();
  }

  // Some keywords have specific token types to differentiate precedence
  if (SQF_Token_Keywords.find(t.raw) != SQF_Token_Keywords.end()) {
    t.type = SQF_Token_Keywords.at(t.raw);
  }
  // Differentiate nullarys for grammar clarity (see issue #11)
  else if (SQF::Nullary_Keywords.find(t.raw) != SQF::Nullary_Keywords.end()) {
    t.type = TokenType::nullary;
  }
  // Remaining keywords may be unary or binary, parser differentiates these
  // (cannot be done here due to ambiguity, consider getDir can be either)
  else if (SQF::Unary_Keywords.find(t.raw) != SQF::Unary_Keywords.end() ||
           SQF::Binary_Keywords.find(t.raw) != SQF::Binary_Keywords.end()) {
    t.type = TokenType::keyword;
  } else {
    t.type = TokenType::id;
  }

  return t;
}

Token Lexer::number() {
  Token t = makeToken(TokenType::dec_literal, "");

  // First determine the type of numeric literal
  if (current_char_ == '$') {
    t.type = TokenType::hex_literal;
    advance();
  } else if (current_char_ == '0' && preproc_.peek() == 'x') {
    t.type = TokenType::hex_literal;
    advance();
    advance();
  }

  // Read the characters according to the literal type
  if (t.type == TokenType::hex_literal) {
    while (current_char_ != '\0' && std::isxdigit(current_char_)) {
      t.raw.push_back(current_char_);
      advance();
    }
  } else {
    // Integer part (may not be present)
    while (current_char_ != '\0' && std::isdigit(current_char_)) {
      t.raw.push_back(current_char_);
      advance();
    }

    // Possible decimal point (can appear at the start or end)
    if (current_char_ == '.') {
      t.raw.push_back(current_char_);
      advance();
    }

    // Possible fractional part
    while (current_char_ != '\0' && std::isdigit(current_char_)) {
      t.raw.push_back(current_char_);
      advance();
    }

    // Optional scientific notation suffix
    if (std::tolower(current_char_) == 'e') {
      t.raw.push_back(current_char_);
      advance();

      // Can be followed by an optional + or -
      if (current_char_ == '+' || current_char_ == '-') {
        t.raw.push_back(current_char_);
        advance();
      }

      // *Must* be followed by digits if present
      if (std::isdigit(current_char_)) {
        while (current_char_ != '\0' && std::isdigit(current_char_)) {
          t.raw.push_back(current_char_);
          advance();
        }
      } else {
        error(t, "Unfinished numeric literal '" + t.raw + "'");
      }
    }
  }

  return t;
}

Token Lexer::string() {
  char enclosing = current_char_;
  Token t = makeToken(TokenType::unknown, "");

  // Skip past beginning enclosing character
  advance();

  // Doubled enclosing character becomes single within string
  while (current_char_ != enclosing || preproc_.peek() == enclosing) {
    // Unclosed string cannot be tokenised
    if (current_char_ == '\0') {
      error(t, "Unclosed string");
    }

    if (current_char_ == enclosing && preproc_.peek() == enclosing) {
      advance();
    }

    t.raw.push_back(current_char_);
    advance();
  }

  // Skip past end enclosing character
  advance();

  t.type = TokenType::str_literal;
  return t;
}

// Convenience function to make a token with current lexer position
Token Lexer::makeToken(TokenType type, std::string raw) {
  Token t;
  t.type = type;
  t.raw = raw;
  t.line = current_char_.line;
  t.column = current_char_.column;
  t.file = current_char_.file;
  return t;
}

Token Lexer::nextToken() {
  while (current_char_ != '\0') {
    // Whitespace is irrelevant in SQF
    if (std::isspace(current_char_)) {
      skipWhitespace();
      continue;
    }

    if (std::isalpha(current_char_) || current_char_ == '_') {
      return _id();
    }

    // Multiple numeric literal formats
    if (std::isdigit(current_char_)
        // Decimal literals can start with the decimal point
        || (current_char_ == '.' && std::isdigit(preproc_.peek()))
        // Hexadecimal literals can start with the dollar sign (or 0x)
        || (current_char_ == '$' && std::isxdigit(preproc_.peek()))) {
      return number();
    }

    if (current_char_ == '"' || current_char_ == '\'') {
      return string();
    }

    if (current_char_ == '=') {
      advance();
      if (current_char_ == '=') {
        advance();
        return makeToken(TokenType::eql, "==");
      }

      return makeToken(TokenType::assign, "=");
    }

    if (current_char_ == '!') {
      advance();
      if (current_char_ == '=') {
        advance();
        return makeToken(TokenType::neql, "!=");
      }

      return makeToken(TokenType::negation, "!");
    }

    if (current_char_ == '>') {
      advance();
      if (current_char_ == '=') {
        advance();
        return makeToken(TokenType::gteql, ">=");
      }
      if (current_char_ == '>') {
        advance();
        return makeToken(TokenType::gtgt, ">>");
      }

      return makeToken(TokenType::gt, ">");
    }

    if (current_char_ == '<') {
      advance();
      if (current_char_ == '=') {
        advance();
        return makeToken(TokenType::lteql, "<=");
      }

      return makeToken(TokenType::lt, "<");
    }

    if (current_char_ == '|' && preproc_.peek() == '|') {
      advance();
      advance();
      return makeToken(TokenType::disjunction, "||");
    }

    if (current_char_ == '&' && preproc_.peek() == '&') {
      advance();
      advance();
      return makeToken(TokenType::conjunction, "&&");
    }

    // General handling of single character tokens
    if (SQF_Token_Chars.find(current_char_) != SQF_Token_Chars.end()) {
      Token t = makeToken(SQF_Token_Chars.at(current_char_),
                          std::string(1, current_char_));

      advance();

      return t;
    }

    error(makeToken(TokenType::unknown, std::string(1, current_char_)),
          "Unexpected character '" + std::string(1, current_char_) + "'");
  }

  return makeToken(TokenType::end_of_file, "");
}

Token Lexer::get() {
  init();

  // Pull from the buffer first if any peek has occured
  if (!peek_buffer_.empty()) {
    Token t = peek_buffer_.front();
    peek_buffer_.pop_front();
    return t;
  }

  return nextToken();
}

Token Lexer::peek(size_t peek_by) {
  init();

  while (peek_buffer_.size() < peek_by) {
    peek_buffer_.push_back(nextToken());
  }

  // Convert peek request to 0-indexed
  return peek_buffer_.at(peek_by - 1);
}
