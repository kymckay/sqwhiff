#pragma once
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../lexer/token_types.h"

class SyntaxError : public std::runtime_error {
 public:
  static const std::string id;

  const int line;
  const int col;
  const std::string file;

  SyntaxError(int line, int col, std::string file, std::string msg)
      : std::runtime_error(msg), line(line), col(col), file(file) {}

  std::string pretty() const;
};

const std::unordered_map<TokenType, std::string> SQF_Token_Descriptors{
    {TokenType::id,
     "a variable, literal or expression"},  // Variable is the final fallback
                                            // default
    {TokenType::nullary, "a command"},
    {TokenType::keyword, "a command"},
    {TokenType::end_of_file, "the end of the file"},
    {TokenType::rparen, "a closing round bracket"},
    {TokenType::rsqb, "a closing square bracket"},
    {TokenType::rcurl, "a closing curly bracket"},
};
