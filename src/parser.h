#pragma once
#include "lexer.h"
#include "token.h"
#include <string>

class Parser
{
    bool lineComment_ = false; // whether the parser is in a single-line comment
    bool blockComment_ = false; // whether the parser is in a block comment
    Token inString_; // the currently parsed string's content

public:
    Parser();
    void parse(Lexer &);
};
