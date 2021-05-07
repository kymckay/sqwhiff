#pragma once
#include "src/errors/error_type.h"
#include <unordered_map>
#include <string>

const std::unordered_map<ErrorType, std::string> ErrorMessages{
    {ErrorType::unexpected_character, "LexerError: Unexpected character"},
    {ErrorType::unclosed_string, "LexerError: Unclosed string"},
    {ErrorType::incomplete_sci, "LexerError: Unfinished numeric literal"},
};