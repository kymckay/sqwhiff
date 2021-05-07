#pragma once

enum class ErrorType
{
    unknown = -1,
    // Lexer Errors
    unexpected_character,
    unclosed_string,
    incomplete_sci,
    // Parser errors
    unexpected_token,
    // Semantic errors
};