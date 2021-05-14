#pragma once
#include <string>
#include <stdexcept>

class PreprocessingError : public std::runtime_error
{
public:
    PreprocessingError(int line, int col, std::string msg) : std::runtime_error(std::to_string(line) + ":" + std::to_string(col) + " PreprocessingError - " + msg) {}
};