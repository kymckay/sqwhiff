#pragma once
#include "token.h"
#include <string>
#include <array>
#include <fstream>

#define NUM_DELIMITERS 35
#define MAX_DELIM_LENGTH 2

class Lexer
{
    const static std::array<std::string, NUM_DELIMITERS> delimiters_;

    std::ifstream file_;
    std::string text_buffer_; // Multiple character delimiters introduce need to buffer text while parsing

    // Current line lexer has reached in the text (1-indexed)
    // Used to give tokens a position for later parsing errors
    int line_ = 1;

    std::string::size_type findNextBoundary(const std::string &);
    int bufferedCharCount(const char);
    bool isDelimiter(const std::string &);

public:
    Lexer(std::string);
    ~Lexer();
    Token nextToken();
};