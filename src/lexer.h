#pragma once
#include "token.h"
#include <string>
#include <vector>
#include <fstream>

#define MAX_DELIM_LENGTH 2

class Lexer
{
    const static std::vector<std::string> DELIMITERS;

    std::ifstream file_;
    std::string text_buffer_; // Multiple character delimiters introduce need to buffer text while parsing

    // Current line lexer has reached in the text (1-indexed)
    // Used to give tokens a position for later parsing errors
    int line_ = 1;

    std::string::size_type findNextBoundary(const std::string &);
    int bufferedCharCount(const char c);

public:
    Lexer(std::string);
    ~Lexer();
    Token nextToken();
};