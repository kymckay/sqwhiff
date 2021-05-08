#pragma once
#include "src/errors/error_type.h"
#include <unordered_map>
#include <string>

const std::unordered_map<ErrorType, std::string> ErrorMessages{
    // Lexer errors
    {ErrorType::unexpected_character, "LexerError: Unexpected character"},
    {ErrorType::unclosed_string, "LexerError: Unclosed string"},
    {ErrorType::incomplete_sci, "LexerError: Unfinished numeric literal"},
    // Parser errors
    {ErrorType::unexpected_token, "SyntaxError: Unexpected token"},
};