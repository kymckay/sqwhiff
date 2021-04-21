#pragma once
#include <string>
#include <vector>
#include <fstream>

#define MAX_DELIM_LENGTH 2

class Lexer
{
    const static std::vector<std::string> DELIMITERS;

    std::ifstream file_;
    std::string text_buffer_; // Multiple character delimiters introduce need to buffer text while parsing

    std::string::size_type findNextBoundary(const std::string &);

public:
    Lexer(std::string);
    ~Lexer();
    std::string nextToken();
};