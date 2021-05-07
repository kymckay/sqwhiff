#pragma once
#include "src/errors/error_type.h"
#include "src/lexer/token.h"

struct Error
{
    ErrorType type = ErrorType::unknown;
    Token token;
};

// Using a typedef prevents naming clashes in global name space
typedef Error Error;