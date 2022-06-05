#pragma once
#include <string>
#include <unordered_map>

#include "../lexer/token_types.h"
#include "./base_error.h"

namespace sqwhiff {

class SyntaxError : public BaseError {
 public:
  using BaseError::BaseError;
  using BaseError::pretty;

  std::string type() const override;
};

}  // namespace sqwhiff

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
