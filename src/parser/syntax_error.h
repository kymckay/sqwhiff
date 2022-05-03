#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../lexer/token_types.h"

class SyntaxError : public std::runtime_error {
 public:
  SyntaxError(int line, int col, std::string msg)
      : std::runtime_error(std::to_string(line) + ":" + std::to_string(col) +
                           " SyntaxError - " + msg) {}
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
