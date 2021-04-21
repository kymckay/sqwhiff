#pragma once
#include <string>
#include <vector>
#include <fstream>

class Lexer
{
    std::ifstream file_;
    std::vector<std::string> tokens_; // May need to store tokens in a buffer

    const static std::vector<std::string> DELIMITERS;
    const static int DELIMITER_MAX_LENGTH = 2;

public:
    Lexer(std::string);
    ~Lexer();
    std::string nextToken();
};