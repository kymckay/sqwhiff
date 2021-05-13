#pragma once
#include "src/preprocessor/pos_char.h"
#include "src/preprocessor/preprocessing_error.h"
#include <string>
#include <istream>

class Preprocessor
{
    // Reference member for polymorphism
    std::istream &stream_;

    char current_char_ = '\0';
    // Current physical position preprocessor has reached
    // Used to give a position to errors and macros
    int lineno_ = 1;
    int column_ = 1;

    // Preprocessor directives must appear at a line start (ignoring whitespace)
    bool line_start_ = true;

    void error(PosChar, std::string);
    void advance();
    void skipComment();

public:
    Preprocessor(std::istream&);
    PosChar get();
    char peek();
};